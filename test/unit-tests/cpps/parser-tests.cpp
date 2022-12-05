#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <tuple>

#include "cpps/check-diagnosis.hpp"
#include "cpps/cst.hpp"
#include "cpps/diagnosis.hpp"
#include "cpps/lexer.hpp"
#include "cpps/parser.hpp"
#include "cpps/source.hpp"
#include "cpps/tokens.hpp"

namespace CPPS {

template<typename T>
concept Stringable = std::is_convertible_v<T, std::string>;

template<Stringable... LinesT>
auto parse(LinesT&&... lines)
{
    Diagnosis diagnosis;

    Source source;
    (source.add(std::string(std::forward<LinesT>(lines)), Source::Line::Type::Cpps), ...);

    Lexer lexer{diagnosis, source};

    Tokens tokens = lexer.lex();

    Parser parser{diagnosis, tokens};

    CST::TranslationUnit tu = parser.parse();

    return std::make_tuple(std::move(source), std::move(diagnosis), std::move(tokens), std::move(tu));
}

template<typename T>
const T& getAs(auto* value)
{
    REQUIRE(value);

    return getAs<T>(*value);
}

template<typename T>
const T& getAs(auto& value)
{
    REQUIRE(value.template is<T>());

    return value.template as<T>();
}

void checkTerm(const CST::ExpressionTerm& term, ParameterModifier parameterModifier, std::string_view value)
{
    CHECK(term.modifier == parameterModifier);

    const CST::IdentifierExpression& identifierExpression = getAs<CST::IdentifierExpression>(term.expression->assignment->primary->type);
    const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(identifierExpression.identifier.type);

    CHECK(unqualifiedIdentifier.identifier.get().text == value);
}

TEST_CASE("Parser identifier-expression", "[Parser], [CST]")
{
    SECTION("unqualified-identifier")
    {
        SECTION("raw")
        {
            const auto [source, diagnosis, tokens, tu] = parse("my_var: int;");

            checkNoErrorOrWarning(diagnosis);

            REQUIRE(tu.declarations.size() == 1);

            const CST::Declaration& declaration = tu.declarations.back();

            CHECK(declaration.identifier->identifier.get().text == "my_var");

            const CST::IdentifierExpression& identifierExpression = getAs<CST::IdentifierExpression>(declaration.type);
            const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(identifierExpression.identifier.type);

            CHECK(unqualifiedIdentifier.identifier.get().text == "int");

            CHECK_FALSE(declaration.initializer.has_value());

            CHECK(declaration.startLocation == SourceLocation{0, 0});
            CHECK(declaration.endLocation == SourceLocation{0, 11});
            CHECK(declaration.equalLocation == InvalidSourceLocation);
        }

        SECTION("pointer declaration")
        {
            const auto [source, diagnosis, tokens, tu] = parse("my_var: *int;");

            checkNoErrorOrWarning(diagnosis);

            REQUIRE(tu.declarations.size() == 1);

            const CST::Declaration& declaration = tu.declarations.back();

            CHECK(declaration.identifier->identifier.get().text == "my_var");

            const CST::IdentifierExpression& identifierExpression = getAs<CST::IdentifierExpression>(declaration.type);
            const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(identifierExpression.identifier.type);

            CHECK(unqualifiedIdentifier.identifier.get().text == "int");

            CHECK_FALSE(declaration.initializer.has_value());

            CHECK(declaration.startLocation == SourceLocation{0, 0});
            CHECK(declaration.endLocation == SourceLocation{0, 12});
            CHECK(declaration.equalLocation == InvalidSourceLocation);

            REQUIRE(declaration.pointerDeclaration.has_value());

            CHECK(declaration.pointerDeclaration.value().get().lexeme == Punctuator::Multiply);
        }
    }
}

TEST_CASE("Parser function-signature", "[Parser], [CST]")
{
    SECTION("empty")
    {
        const auto [source, diagnosis, tokens, tu] = parse("my_func: () -> void = {}");

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();

        CHECK(declaration.startLocation == SourceLocation{0, 0});
        CHECK(declaration.endLocation == SourceLocation{0, 23});
        CHECK(declaration.equalLocation == SourceLocation{0, 20});

        const CST::FunctionSignature& functionSignature = getAs<CST::FunctionSignature>(declaration.type);

        CHECK(functionSignature.parameters.openParenthesisLocation == SourceLocation{0, 9});
        CHECK(functionSignature.parameters.closeParenthesisLocation == SourceLocation{0, 10});

        CHECK(functionSignature.parameters.empty());

        CHECK_FALSE(functionSignature.throws);
    }

    SECTION("throws")
    {
        const auto [source, diagnosis, tokens, tu] = parse("my_func: () throws -> void = {}");

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();

        CHECK(declaration.startLocation == SourceLocation{0, 0});
        CHECK(declaration.endLocation == SourceLocation{0, 30});
        CHECK(declaration.equalLocation == SourceLocation{0, 27});

        const CST::FunctionSignature& functionSignature = getAs<CST::FunctionSignature>(declaration.type);

        CHECK(functionSignature.parameters.openParenthesisLocation == SourceLocation{0, 9});
        CHECK(functionSignature.parameters.closeParenthesisLocation == SourceLocation{0, 10});

        CHECK(functionSignature.parameters.empty());

        CHECK(functionSignature.throws);
    }

    SECTION("parameter parameter_modifier")
    {
        auto check =
            [](const std::string& info, const std::string& code, ParameterModifier parameterModifier, SourceLocation openParenthesis, SourceLocation closeParenthesis, SourceLocation equalLocation, SourceLocation endLocation) {
                INFO(info);

                const auto [source, diagnosis, tokens, tu] = parse(code);

                checkNoErrorOrWarning(diagnosis);

                const CST::Declaration& declaration = tu.declarations.back();

                CHECK(declaration.startLocation == SourceLocation{0, 0});
                CHECK(declaration.endLocation == endLocation);
                CHECK(declaration.equalLocation == equalLocation);

                const CST::FunctionSignature& functionSignature = getAs<CST::FunctionSignature>(declaration.type);

                CHECK(functionSignature.parameters.openParenthesisLocation == openParenthesis);
                CHECK(functionSignature.parameters.closeParenthesisLocation == closeParenthesis);

                REQUIRE(functionSignature.parameters.size() == 1);

                const CST::ParameterDeclaration& parameterDeclaration = functionSignature.parameters[0];

                CHECK(parameterDeclaration.modifier == parameterModifier);
            };

        check("copy", "my_func: (copy p: int) -> void = {}", ParameterModifier::Copy, {0, 9}, {0, 21}, {0, 31}, {0, 34});
        check("forward", "my_func: (forward p: int) -> void = {}", ParameterModifier::Forward, {0, 9}, {0, 24}, {0, 34}, {0, 37});
        check("in implicit", "my_func: (p: int) -> void = {}", ParameterModifier::In, {0, 9}, {0, 16}, {0, 26}, {0, 29});
        check("in explicit", "my_func: (in p: int) -> void = {}", ParameterModifier::In, {0, 9}, {0, 19}, {0, 29}, {0, 32});
        check("inout", "my_func: (inout p: int) -> void = {}", ParameterModifier::InOut, {0, 9}, {0, 22}, {0, 32}, {0, 35});
        check("move", "my_func: (move p: int) -> void = {}", ParameterModifier::Move, {0, 9}, {0, 21}, {0, 31}, {0, 34});
        check("out", "my_func: (out p: int) -> void = {}", ParameterModifier::Out, {0, 9}, {0, 20}, {0, 30}, {0, 33});
    }

    SECTION("multiple parameters")
    {
        const auto [source, diagnosis, tokens, tu] = parse("my_func: (p0: int, in p1: short, inout p2: int, out p3: short, move p4: int, forward p5: short) -> void = { }");

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();

        CHECK(declaration.startLocation == SourceLocation{0, 0});
        CHECK(declaration.endLocation == SourceLocation{0, 108});
        CHECK(declaration.equalLocation == SourceLocation{0, 104});

        const CST::FunctionSignature& functionSignature = getAs<CST::FunctionSignature>(declaration.type);

        CHECK(functionSignature.parameters.openParenthesisLocation == SourceLocation{0, 9});
        CHECK(functionSignature.parameters.closeParenthesisLocation == SourceLocation{0, 94});

        REQUIRE(functionSignature.parameters.size() == 6);

        auto check = [&functionSignature](std::size_t index, ParameterModifier parameterModifier, std::string_view name, std::string_view type, SourceLocation location) {
            INFO(fmt::format("{} ({} {}: {})", index, toStringView(parameterModifier), name, type));

            REQUIRE(index < functionSignature.parameters.size());

            const CST::ParameterDeclaration& parameterDeclaration = functionSignature.parameters[index];

            REQUIRE(parameterDeclaration.declaration);

            CHECK(parameterDeclaration.modifier == parameterModifier);
            CHECK(parameterDeclaration.location == location);

            const CST::Declaration& decl = *parameterDeclaration.declaration;

            CHECK(decl.identifier->identifier.get().text == name);

            const CST::IdentifierExpression& identifierExpression = getAs<CST::IdentifierExpression>(decl.type);
            const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(identifierExpression.identifier.type);

            CHECK(unqualifiedIdentifier.identifier.get().text == type);
        };

        check(0, ParameterModifier::In, "p0", "int", {0, 10});
        check(1, ParameterModifier::In, "p1", "short", {0, 19});
        check(2, ParameterModifier::InOut, "p2", "int", {0, 33});
        check(3, ParameterModifier::Out, "p3", "short", {0, 48});
        check(4, ParameterModifier::Move, "p4", "int", {0, 63});
        check(5, ParameterModifier::Forward, "p5", "short", {0, 77});
    }
}

TEST_CASE("parser return statement", "[Parser], [CST]")
{
    SECTION("non empty")
    {
        const auto [source, diagnosis, tokens, tu] = parse("my_func: () -> int = { return 42; }");

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();

        CHECK(declaration.startLocation == SourceLocation{0, 0});
        CHECK(declaration.endLocation == SourceLocation{0, 34});
        CHECK(declaration.equalLocation == SourceLocation{0, 19});

        REQUIRE(declaration.initializer.has_value());

        const CST::CompoundStatement& compoundStatement = getAs<CST::CompoundStatement>(declaration.initializer.value()->type);

        REQUIRE(compoundStatement.size() == 1);

        const CST::ReturnStatement& returnStatement = getAs<CST::ReturnStatement>(compoundStatement[0].type);

        REQUIRE(returnStatement.expression);

        CHECK(getAs<Token>(returnStatement.expression->assignment->primary->type).text == "42");
    }

    SECTION("empty")
    {
        const auto [source, diagnosis, tokens, tu] = parse("my_func: () -> int = { return; }");

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();

        CHECK(declaration.startLocation == SourceLocation{0, 0});
        CHECK(declaration.endLocation == SourceLocation{0, 31});
        CHECK(declaration.equalLocation == SourceLocation{0, 19});

        REQUIRE(declaration.initializer.has_value());

        const CST::CompoundStatement& compoundStatement = getAs<CST::CompoundStatement>(declaration.initializer.value()->type);

        REQUIRE(compoundStatement.size() == 1);

        const CST::ReturnStatement& returnStatement = getAs<CST::ReturnStatement>(compoundStatement[0].type);

        CHECK_FALSE(returnStatement.expression);
    }
}

TEST_CASE("Parser expression statement", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse("my_var: int = 42;");

    checkNoErrorOrWarning(diagnosis);

    const CST::Declaration& declaration = tu.declarations.back();

    CHECK(declaration.startLocation == SourceLocation{0, 0});
    CHECK(declaration.endLocation == SourceLocation{0, 16});
    CHECK(declaration.equalLocation == SourceLocation{0, 12});

    const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);

    CHECK(expressionStatement.hasSemicolon);

    CHECK(getAs<Token>(expressionStatement.expression->assignment->primary->type).text == "42");
}

TEST_CASE("Parser assignation expression", "[Parser], [CST]")
{
    auto check = [](std::string info, std::string code, std::string_view lhs_v, Punctuator punctuator, std::string_view rhs_v) {
        INFO(info);

        const auto [source, diagnosis, tokens, tu] = parse(
            "my_func_scope: () = {",
            std::move(code),
            "}");

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();

        REQUIRE(declaration.initializer);

        const CST::CompoundStatement& compoundStatement = getAs<CST::CompoundStatement>(declaration.initializer.value()->type);

        REQUIRE(compoundStatement.size() == 1);

        const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(compoundStatement[0].type);

        CHECK(expressionStatement.hasSemicolon);

        REQUIRE(expressionStatement.expression->assignment->terms.size() == 1);

        const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(
            getAs<CST::IdentifierExpression>(expressionStatement.expression->assignment->primary->type).identifier.type);

        CHECK(unqualifiedIdentifier.identifier.get().text == lhs_v);

        const CST::AssignmentExpression::Term& term = expressionStatement.expression->assignment->terms[0];

        CHECK(term.op.get().lexeme == punctuator);

        CHECK(getAs<Token>(term.primary->type).text == rhs_v);
    };

    check("=", "my_var = 4;", "my_var", Punctuator::Assignment, "4");
    check("+=", "my_var += 4;", "my_var", Punctuator::PlusEqual, "4");
    check("-=", "my_var -= 4;", "my_var", Punctuator::MinusEqual, "4");
    check("*=", "my_var *= 4;", "my_var", Punctuator::MultiplyEqual, "4");
    check("/=", "my_var /= 4;", "my_var", Punctuator::SlashEqual, "4");
    check("^=", "my_var ^= 4;", "my_var", Punctuator::CaretEqual, "4");
    check("%=", "my_var %= 4;", "my_var", Punctuator::ModuloEqual, "4");
    check("&=", "my_var &= 4;", "my_var", Punctuator::AmpersandEqual, "4");
    check("|=", "my_var |= 4;", "my_var", Punctuator::PipeEqual, "4");
    check(">>=", "my_var >>= 4;", "my_var", Punctuator::RightShiftEqual, "4");
    check("<<=", "my_var <<= 4;", "my_var", Punctuator::LeftShiftEqual, "4");
}

TEST_CASE("Parser binary expression", "[Parser], [CST]")
{
    auto check = []<typename Expression>(std::string info, std::string code, std::string_view lhs_v, Punctuator punctuator, std::string_view rhs_v, Expression* = nullptr) {
        INFO(info);

        const auto [source, diagnosis, tokens, tu] = parse(std::move(code));

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();

        const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);

        CHECK(expressionStatement.hasSemicolon);

        CHECK(getAs<Token>(expressionStatement.expression->assignment->primary->type).text == lhs_v);

        const Expression& expression = *expressionStatement.expression->assignment;

        REQUIRE(expression.terms.size() == 1);

        const auto& term = expression.terms[0];

        CHECK(term.op.get().lexeme == punctuator);
        CHECK(getAs<Token>(term.primary->type).text == rhs_v);
    }; // NOLINT(readability/braces)

    check("*", "my_var: int = 4 * 2;", "4", Punctuator::Multiply, "2", static_cast<CST::MultiplicativeExpression*>(nullptr));
    check("/", "my_var: int = 4 / 2;", "4", Punctuator::Slash, "2", static_cast<CST::MultiplicativeExpression*>(nullptr));
    check("+", "my_var: int = 4 + 2;", "4", Punctuator::Plus, "2", static_cast<CST::AdditiveExpression*>(nullptr));
    check("-", "my_var: int = 4 - 2;", "4", Punctuator::Minus, "2", static_cast<CST::AdditiveExpression*>(nullptr));
    check(">>", "my_var: int = 4 >> 2;", "4", Punctuator::RightShift, "2", static_cast<CST::ShiftExpression*>(nullptr));
    check("<<", "my_var: int = 4 << 2;", "4", Punctuator::LeftShift, "2", static_cast<CST::ShiftExpression*>(nullptr));
    check("<=>", "my_var: int = 4 <=> 2;", "4", Punctuator::Spaceship, "2", static_cast<CST::CompareExpression*>(nullptr));
    check("<", "my_var: int = 4 < 2;", "4", Punctuator::Less, "2", static_cast<CST::RelationalExpression*>(nullptr));
    check(">", "my_var: int = 4 > 2;", "4", Punctuator::Greater, "2", static_cast<CST::RelationalExpression*>(nullptr));
    check("<=", "my_var: int = 4 <= 2;", "4", Punctuator::LessEqual, "2", static_cast<CST::RelationalExpression*>(nullptr));
    check(">=", "my_var: int = 4 >= 2;", "4", Punctuator::GreaterEqual, "2", static_cast<CST::RelationalExpression*>(nullptr));
    check("==", "my_var: int = 4 == 2;", "4", Punctuator::CompareEqual, "2", static_cast<CST::EqualityExpression*>(nullptr));
    check("!=", "my_var: int = 4 != 2;", "4", Punctuator::CompareNotEqual, "2", static_cast<CST::EqualityExpression*>(nullptr));
    check("&", "my_var: int = 4 & 2;", "4", Punctuator::Ampersand, "2", static_cast<CST::BitAndExpression*>(nullptr));
    check("^", "my_var: int = 4 ^ 2;", "4", Punctuator::Caret, "2", static_cast<CST::BitXorExpression*>(nullptr));
    check("|", "my_var: int = 4 | 2;", "4", Punctuator::Pipe, "2", static_cast<CST::BitOrExpression*>(nullptr));
    check("&&", "my_var: int = 4 && 2;", "4", Punctuator::LogicalAnd, "2", static_cast<CST::LogicalAndExpression*>(nullptr));
    check("||", "my_var: int = 4 || 2;", "4", Punctuator::LogicalOr, "2", static_cast<CST::LogicalOrExpression*>(nullptr));
}

TEST_CASE("Parser prefix expression", "[Parser], [CST]")
{
    auto check = [](std::string info, std::string_view code, Punctuator punctuator) {
        INFO(info);

        const auto [source, diagnosis, tokens, tu] = parse(std::string(code));

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();
        const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
        const CST::PrefixExpression& prefix = *expressionStatement.expression->assignment->prefix;

        REQUIRE(prefix.ops.size() == 1);
        CHECK(prefix.ops[0].get().lexeme == punctuator);

        const CST::IdentifierExpression& identifierExpression = getAs<CST::IdentifierExpression>(expressionStatement.expression->assignment->primary->type);
        const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(identifierExpression.identifier.type);

        CHECK(unqualifiedIdentifier.identifier.get().text == "i");
    };

    check("!", "my_var: bool = !i;", Punctuator::Not);
    check("+", "my_var: bool = +i;", Punctuator::Plus);
    check("-", "my_var: bool = -i;", Punctuator::Minus);
}

TEST_CASE("Parser postfix", "[Parser], [CST]")
{
    SECTION("postfix-operator")
    {
        auto check = [](std::string_view info, std::string_view code, Punctuator punctuator) {
            INFO(info);

            const auto [source, diagnosis, tokens, tu] = parse(std::string(code));

            checkNoErrorOrWarning(diagnosis);

            const CST::Declaration& declaration = tu.declarations.back();
            const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
            const CST::PostfixExpression& postfix = *expressionStatement.expression->assignment->postfix;

            REQUIRE(postfix.terms.size() == 1);
            CHECK(postfix.terms[0].op.get().lexeme == punctuator);

            const CST::IdentifierExpression& identifierExpression = getAs<CST::IdentifierExpression>(expressionStatement.expression->assignment->primary->type);
            const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(identifierExpression.identifier.type);

            CHECK(unqualifiedIdentifier.identifier.get().text == "i");
        };

        check("++", "my_var: int = i++;", Punctuator::PlusPlus);
        check("--", "my_var: int = i--;", Punctuator::MinusMinus);
        check("*", "my_var: int = i*;", Punctuator::Multiply);
        check("&", "my_var: int = i&;", Punctuator::Ampersand);
        check("~", "my_var: int = i~;", Punctuator::Tilde);
    }

    SECTION("[ expression-list ]")
    {
        const auto [source, diagnosis, tokens, tu] = parse(R"(my_var: int = my_func[a, b];)");

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();
        const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
        const CST::PostfixExpression& postfix = *expressionStatement.expression->assignment->postfix;

        REQUIRE(postfix.terms.size() == 1);

        const CST::PostfixExpression::Term& term = postfix.terms[0];

        REQUIRE(term.closeOp);

        CHECK(term.op.get().lexeme == Punctuator::OpenBracket);
        CHECK(term.closeOp->get().lexeme == Punctuator::CloseBracket);

        REQUIRE(term.expressions);
        const CST::ExpressionList& expressions = *term.expressions;

        CHECK(expressions.openParenthesisLocation == SourceLocation{0, 21});
        CHECK(expressions.closeParenthesisLocation == SourceLocation{0, 26});

        REQUIRE(expressions.size() == 2);

        checkTerm(expressions[0], ParameterModifier::In, "a");
        checkTerm(expressions[1], ParameterModifier::In, "b");
    }

    SECTION("( expression-list? )")
    {
        SECTION("()")
        {
            const auto [source, diagnosis, tokens, tu] = parse(R"(my_var: int = my_func();)");

            checkNoErrorOrWarning(diagnosis);

            const CST::Declaration& declaration = tu.declarations.back();
            const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
            const CST::PostfixExpression& postfix = *expressionStatement.expression->assignment->postfix;

            REQUIRE(postfix.terms.size() == 1);

            const CST::PostfixExpression::Term& term = postfix.terms[0];

            REQUIRE(term.closeOp);

            CHECK(term.op.get().lexeme == Punctuator::OpenParenthesis);
            CHECK(term.closeOp.value().get().lexeme == Punctuator::CloseParenthesis);

            const CST::ExpressionList& expressions = *term.expressions;

            CHECK(expressions.openParenthesisLocation == SourceLocation{0, 21});
            CHECK(expressions.closeParenthesisLocation == SourceLocation{0, 22});
            CHECK(expressions.empty());
        }

        SECTION("( expression-list )")
        {
            const auto [source, diagnosis, tokens, tu] = parse(R"(my_var: int = my_func(a, b);)");

            checkNoErrorOrWarning(diagnosis);

            const CST::Declaration& declaration = tu.declarations.back();
            const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
            const CST::PostfixExpression& postfix = *expressionStatement.expression->assignment->postfix;

            REQUIRE(postfix.terms.size() == 1);

            const CST::PostfixExpression::Term& term = postfix.terms[0];

            REQUIRE(term.closeOp);

            CHECK(term.op.get().lexeme == Punctuator::OpenParenthesis);
            CHECK(term.closeOp.value().get().lexeme == Punctuator::CloseParenthesis);

            const CST::ExpressionList& expressions = *term.expressions;

            CHECK(expressions.openParenthesisLocation == SourceLocation{0, 21});
            CHECK(expressions.closeParenthesisLocation == SourceLocation{0, 26});

            REQUIRE(expressions.size() == 2);

            checkTerm(expressions[0], ParameterModifier::In, "a");
            checkTerm(expressions[1], ParameterModifier::In, "b");
        }

        SECTION(". id-expression")
        {
            const auto [source, diagnosis, tokens, tu] = parse(R"(my_var: int = my_obj.my_member;)");

            checkNoErrorOrWarning(diagnosis);

            const CST::Declaration& declaration = tu.declarations.back();
            const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
            const CST::PostfixExpression& postfix = *expressionStatement.expression->assignment->postfix;

            REQUIRE(postfix.terms.size() == 1);

            const CST::PostfixExpression::Term& term = postfix.terms[0];

            CHECK(term.op.get().lexeme == Punctuator::Dot);
            CHECK_FALSE(term.closeOp.has_value());

            const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(term.identifierExpression->identifier.type);

            CHECK(unqualifiedIdentifier.identifier.get().text == "my_member");
        }
    }
}

TEST_CASE("Parser primary-expression", "[Parser], [CST]")
{
    SECTION("literal")
    {
        SECTION("boolean")
        {
            const auto [source, diagnosis, tokens, tu] = parse(std::string("my_var: bool = true;"));

            checkNoErrorOrWarning(diagnosis);

            const CST::Declaration& declaration = tu.declarations.back();
            const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
            const CST::PrimaryExpression& primaryExpression = *expressionStatement.expression->assignment->primary;

            CHECK(getAs<Token>(primaryExpression.type).lexeme == BooleanLiteral::True);
        }

        SECTION("pointer")
        {
            const auto [source, diagnosis, tokens, tu] = parse(std::string("my_var: bool = nullptr;"));

            checkNoErrorOrWarning(diagnosis);

            const CST::Declaration& declaration = tu.declarations.back();
            const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
            const CST::PrimaryExpression& primaryExpression = *expressionStatement.expression->assignment->primary;

            CHECK(getAs<Token>(primaryExpression.type).lexeme == PointerLiteral::NullPtr);
        }

        SECTION("integer")
        {
        }

        SECTION("floating")
        {
        }

        SECTION("character")
        {
        }

        SECTION("string")
        {
        }
    }

    SECTION("( expression-list )")
    {
        const auto [source, diagnosis, tokens, tu] = parse(std::string("my_var: int = ( i );"));

        checkNoErrorOrWarning(diagnosis);

        const CST::Declaration& declaration = tu.declarations.back();
        const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
        const CST::ExpressionList& expressions = getAs<CST::ExpressionList>(expressionStatement.expression->assignment->primary->type);

        REQUIRE(expressions.size() == 1);

        const CST::IdentifierExpression& identifierExpression = getAs<CST::IdentifierExpression>(expressions[0].expression->assignment->primary->type);
        const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(identifierExpression.identifier.type);

        CHECK(unqualifiedIdentifier.identifier.get().text == "i");
    }

    SECTION("id-expression")
    {
        SECTION("unnamed-declaration")
        {
            const auto [source, diagnosis, tokens, tu] = parse("my_var: int = i;");

            checkNoErrorOrWarning(diagnosis);

            const CST::Declaration& declaration = tu.declarations.back();

            const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
            const CST::IdentifierExpression& identifierExpression = getAs<CST::IdentifierExpression>(expressionStatement.expression->assignment->primary->type);
            const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(identifierExpression.identifier.type);

            CHECK(unqualifiedIdentifier.identifier.get().text == "i");
        }
    }
}

TEST_CASE("Parser expression list", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse(R"(my_var: int = my_func(j, move m, out o);)");

    checkNoErrorOrWarning(diagnosis);

    const CST::Declaration& declaration = tu.declarations.back();

    const CST::ExpressionStatement& expressionStatement = getAs<CST::ExpressionStatement>(declaration.initializer.value()->type);
    const CST::IdentifierExpression& identifierExpression = getAs<CST::IdentifierExpression>(expressionStatement.expression->assignment->primary->type);
    const CST::UnqualifiedIdentifier& unqualifiedIdentifier = getAs<CST::UnqualifiedIdentifier>(identifierExpression.identifier.type);

    CHECK(unqualifiedIdentifier.identifier.get().text == "my_func");

    REQUIRE(expressionStatement.expression->assignment->postfix->terms.size() == 1);

    const auto& term = expressionStatement.expression->assignment->postfix->terms[0];

    CHECK(term.op.get().lexeme == Punctuator::OpenParenthesis);

    const CST::ExpressionList& expressions = *term.expressions;

    REQUIRE(expressions.size() == 3);

    checkTerm(expressions[0], ParameterModifier::In, "j");
    checkTerm(expressions[1], ParameterModifier::Move, "m");
    checkTerm(expressions[2], ParameterModifier::Out, "o");
}

TEST_CASE("Parser diagnosis dot must be followed by valid member name", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse(R"(my_var: int = my_obj.;)");

    checkNoWarning(diagnosis);
    checkError(diagnosis, Parser::DiagnosisMessage::dotMustFollowedByValidMemberName(), SourceLocation{0, 20});
}

TEST_CASE("Parser diagnosis invalid return expression", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse("my_func: () -> int = { return }");

    checkNoWarning(diagnosis);
    checkError(diagnosis, Parser::DiagnosisMessage::invalidReturnExpression(), SourceLocation{0, 30});
}

TEST_CASE("Parser diagnosis invalid return parameter modifier", "[Parser], [CST]")
{
    auto check = [](ParameterModifier parameterModifier) {
        const auto [source, diagnosis, tokens, tu] = parse(fmt::format("my_func: () -> ({} a: int) = {{ }}", parameterModifier));

        checkNoWarning(diagnosis);
        checkError(diagnosis, Parser::DiagnosisMessage::invalidReturnParameterModifier(parameterModifier), SourceLocation{0, 16});
    };

    check(ParameterModifier::In);
    check(ParameterModifier::InOut);
    check(ParameterModifier::Copy);
    check(ParameterModifier::Move);
}

TEST_CASE("Parser diagnosis missing semicolon at end declaration", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse(R"(my_var: int)");

    checkNoWarning(diagnosis);
    checkError(diagnosis, Parser::DiagnosisMessage::missingSemicolonAtEndDeclaration(), SourceLocation{0, 0});
}

TEST_CASE("Parser diagnosis missing semicolon at end statement", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse(R"(my_var: int = 1)");

    checkNoWarning(diagnosis);
    checkError(diagnosis, Parser::DiagnosisMessage::missingSemicolonAtEndStatement(), SourceLocation{0, 14});
}

TEST_CASE("Parser diagnosis missing equal before function body", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse("my_func: () -> void { }");

    checkNoWarning(diagnosis);
    checkError(diagnosis, Parser::DiagnosisMessage::missingEqualBeforeFunctionBody(), SourceLocation{0, 0});
}

TEST_CASE("Parser diagnosis missing comma between parameter declarations", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse("my_func: (p0 : int p1 : int) -> void { }");

    checkNoWarning(diagnosis);
    checkError(diagnosis, Parser::DiagnosisMessage::missingCommaBetweenParameterDeclarations(), SourceLocation{0, 19});
}

TEST_CASE("Parser diagnosis missing close parenthesis for parameter list", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse("my_func: (");

    checkNoWarning(diagnosis);
    checkError(diagnosis, Parser::DiagnosisMessage::missingCloseParenthesisForParameterList(), SourceLocation{0, 9});
}

TEST_CASE("Parser diagnosis subscript expression bracket empty", "[Parser], [CST]")
{
    const auto [source, diagnosis, tokens, tu] = parse(R"(my_var: int = a[];)");

    checkNoWarning(diagnosis);
    checkError(diagnosis, Parser::DiagnosisMessage::subscriptExpressionBracketEmpty(), SourceLocation{0, 15});
}

} // namespace CPPS
