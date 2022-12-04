#include "cpps/parser.hpp"

#include <utility>

#include "cpps/diagnosis.hpp"
#include "cpps/tokens.hpp"

namespace CPPS {

namespace {

template<Punctuator... PunctuatorsT>
constexpr bool anyTokenPunctuator(const Token& token)
{
    if (!token.lexeme.is<Punctuator>())
    {
        return false;
    }

    const Punctuator punctuator = token.lexeme.get<Punctuator>();

    return ((punctuator == PunctuatorsT) || ...);
}

template<Keyword... KeywordsT>
constexpr bool anyTokenKeyword(const Token& token)
{
    if (!token.lexeme.is<Keyword>())
    {
        return false;
    }

    const Keyword keyword = token.lexeme.get<Keyword>();

    return ((keyword == KeywordsT) || ...);
}

} // namespace

std::string Parser::DiagnosisMessage::dotMustFollowedByValidMemberName()
{
    return "'.' must be followed by a valid member name";
}

std::string Parser::DiagnosisMessage::illFormedInitializer()
{
    return "ill-formed initializer";
}

std::string Parser::DiagnosisMessage::invalidExpressionAfter(std::string_view token)
{
    return fmt::format("invalid expression after {}", token);
}

std::string Parser::DiagnosisMessage::invalidReturnExpression()
{
    return "invalid return expression";
}

std::string Parser::DiagnosisMessage::invalidStatementInCompoundStatement()
{
    return "invalid statement in compound-statement";
}

std::string Parser::DiagnosisMessage::invalidTextInExpressionList()
{
    return "invalid text in expression list";
}

std::string Parser::DiagnosisMessage::missingCloseParenthesisForParameterList()
{
    return "missing close parenthesis in parameter list";
}

std::string Parser::DiagnosisMessage::missingCommaBetweenParameterDeclarations()
{
    return "missing comma between parameter declarations";
}

std::string Parser::DiagnosisMessage::missingEqualBeforeFunctionBody()
{
    return "missing = before function body";
}

std::string Parser::DiagnosisMessage::missingFunctionReturnAfterArrow()
{
    return "missing function return after ->";
}

std::string Parser::DiagnosisMessage::missingSemicolonAtEndDeclaration()
{
    return "missing ';' at end of the declaration";
}

std::string Parser::DiagnosisMessage::missingSemicolonAtEndStatement()
{
    return "missing ';' at the end of the statement";
}

std::string Parser::DiagnosisMessage::missingSemicolonEndStatement()
{
    return "missing ';' at end of the return statement";
}

std::string Parser::DiagnosisMessage::parenthesisNotFollowedByExpressionList()
{
    return "( is not followed by a valid expression list";
}

std::string Parser::DiagnosisMessage::subscriptExpressionBracketEmpty()
{
    return "subscript expression [ ] must not be empty";
}

std::string Parser::DiagnosisMessage::unexpectedTextAfterExpressionList()
{
    return "unexpected text - ( is not followed by an expression-list";
}

std::string Parser::DiagnosisMessage::unexpectedTextAfterOpenParenthesis()
{
    return "unexpected text - ( is not followed by an expression-list";
}

std::string Parser::DiagnosisMessage::unexpectedTextBracketDoesNotMatch()
{
    return "unexpected text - [ is not properly matched by ]";
}

std::string Parser::DiagnosisMessage::unexpectedTextParenthesisDoesNotMatch()
{
    return "unexpected text - ( is not properly matched by )";
}

std::string Parser::DiagnosisMessage::unnamedDeclarationAtExpressionScopeMustBeFunction()
{
    return "an unnamed declaration at expression scope must be a function";
}

std::string Parser::DiagnosisMessage::unnamedFunctionAtExpressionScopeCannotReturnsMultipleValues()
{
    return "an unnamed function at expression scope currently cannot return multiple values";
}

Parser::Parser(Diagnosis& diagnosis, const Tokens& tokens)
    : _diagnosis(diagnosis)
    , _tokens(tokens)
{
}

// translation-unit:
//     declaration-seq
//          declaration
//          declaration_seq declaration
CST::TranslationUnit Parser::parse()
{
    while (CST::Node node = parseDeclaration())
    {
        _tu.declarations.add(std::move(node));
    }

    return std::move(_tu);
}

// declaration:
//    identifier unnamed-declaration
CST::Node<CST::Declaration> Parser::parseDeclaration(bool mustEndWithSemicolon)
{
    if (isEnd())
    {
        return {};
    }

    const std::size_t startIndex = _currentTokenIndex;
    const SourceLocation startLocation = current().location;

    if (CST::Node identifier = parseUnqualifiedIdentifier())
    {
        if (CST::Node declaration = parseUnnamedDeclaration(std::move(identifier), startLocation, mustEndWithSemicolon))
        {
            return declaration;
        }
        else
        {
            _currentTokenIndex = startIndex;
        }
    }

    return {};
}

// unnamed-declaration:
//      : function-signature = statement
//      : identifier-expression-opt = statement
//      : identifier-expression
CST::Node<CST::Declaration> Parser::parseUnnamedDeclaration(SourceLocation startLocation, bool mustEndWithSemicolon)
{
    if (current().lexeme != Punctuator::Colon)
    {
        // token must be :
        return {};
    }

    // skip the :
    next();

    CST::Node<CST::Declaration> declaration{allocator()};
    declaration->startLocation = startLocation;

    if (CST::Node functionSignature = parseFunctionSignature())
    {
        declaration->type = std::move(functionSignature);
    }
    else if (current().lexeme == Punctuator::Multiply)
    {
        // pointer object
        declaration->pointerDeclaration = current();

        // skip the ptr decl
        next();

        declaration->type = parseIdentifierExpression();
    }
    else if (CST::Node identifierExpression = parseIdentifierExpression())
    {
        // non-pointer object
        declaration->type = std::move(identifierExpression);
    }
    else
    {
        // TODO deduced type
        return {};
    }

    if (current().lexeme == Punctuator::Assignment)
    {
        declaration->equalLocation = current().location;

        // skip the =
        next();

        declaration->initializer = parseStatement(mustEndWithSemicolon, declaration->equalLocation);

        if (declaration->initializer == std::nullopt)
        {
            _diagnosis.error(DiagnosisMessage::illFormedInitializer(), current().location);
            next();
            return {};
        }
    }
    else
    {
        if (declaration->type.is<CST::FunctionSignature>())
        {
            _diagnosis.error(DiagnosisMessage::missingEqualBeforeFunctionBody(), current().location);
            return {};
        }

        if (current().lexeme == Punctuator::Semicolon)
        {
            // skip the ;
            next();
        }
        else if (mustEndWithSemicolon)
        {
            _diagnosis.error(DiagnosisMessage::missingSemicolonAtEndDeclaration(), current().location);
            return {};
        }
    }

    declaration->endLocation = peekBack(1).location;

    return declaration;
}

// unnamed-declaration:
//     : function-type = statement
//     : id-expression-opt = statement
//     : id-expression
CST::Node<CST::Declaration> Parser::parseUnnamedDeclaration(CST::Node<CST::UnqualifiedIdentifier>&& identifier, SourceLocation startLocation, bool mustEndWithSemicolon)
{
    if (CST::Node declaration = parseUnnamedDeclaration(startLocation, mustEndWithSemicolon))
    {
        declaration->identifier = std::move(identifier);

        return declaration;
    }

    return {};
}

// parameter-declaration-list
//     ( parameter-declaration-seq-opt )
// parameter-declaration-seq:
//     parameter-declaration
//     parameter-declaration-seq , parameter-declaration
bool Parser::parseParameterDeclarationList(CST::ParameterDeclarationList& params)
{
    const Token& openParenthesisToken = current();

    if (openParenthesisToken.lexeme != Punctuator::OpenParenthesis)
    {
        return false;
    }

    params.openParenthesisLocation = openParenthesisToken.location;

    // skip the (
    next();

    while (CST::Node param = parseParameterDeclaration())
    {
        params.add(std::move(param));

        const Token& curr = current();

        if (curr.lexeme == Punctuator::CloseParenthesis)
        {
            break;
        }

        if (curr.lexeme != Punctuator::Comma)
        {
            _diagnosis.error(DiagnosisMessage::missingCommaBetweenParameterDeclarations(), curr.location);
            return false;
        }

        // skip the ,
        next();
    }

    const Token& closeParenthesisToken = current();

    if (closeParenthesisToken.lexeme != Punctuator::CloseParenthesis)
    {
        _diagnosis.error(DiagnosisMessage::missingCloseParenthesisForParameterList(), closeParenthesisToken.location);
        next();
        return false;
    }

    params.closeParenthesisLocation = closeParenthesisToken.location;

    next();

    return true;
}

// parameter-declaration:
//     parameter-direction-opt declaration
// parameter-direction: one of
//     in copy inout out move forward
CST::Node<CST::ParameterDeclaration> Parser::parseParameterDeclaration(bool returns)
{
    const Token& startToken = current();

    ParameterModifier parameterModifier{returns ? ParameterModifier::Out : ParameterModifier::In};

    if (startToken.lexeme.is<ParameterModifier>())
    {
        switch (startToken.lexeme.get<ParameterModifier>())
        {
        case ParameterModifier::In:
        case ParameterModifier::InOut:
        case ParameterModifier::Copy:
        case ParameterModifier::Move:
            if (returns)
            {
                _diagnosis.error(fmt::format("a return value cannot be '{}'", startToken.lexeme.get<ParameterModifier>()), current().location);
                return {};
            }
            parameterModifier = startToken.lexeme.get<ParameterModifier>();
            next();
            break;

        case ParameterModifier::Out:
        case ParameterModifier::Forward:
            parameterModifier = startToken.lexeme.get<ParameterModifier>();
            next();
            break;
        }
    }

    CST::Node decl = parseDeclaration(false);

    if (!decl)
    {
        return {};
    }

    CST::Node<CST::ParameterDeclaration> parameterDeclaration{allocator()};
    parameterDeclaration->declaration = std::move(decl);
    parameterDeclaration->modifier = parameterModifier;
    parameterDeclaration->location = startToken.location;
    return parameterDeclaration;
}

// expression:
//     assignment-expression
// TODO    try expression
CST::Node<CST::Expression> Parser::parseExpression(bool allowRelationalComparison)
{
    CST::Node<CST::AssignmentExpression> assignment{allocator()};

    if (!parseAssignmentExpression(*assignment, allowRelationalComparison))
    {
        return {};
    }

    CST::Node<CST::Expression> expression{allocator()};
    expression->assignment = std::move(assignment);
    return expression;
}

// expression-list:
//     expression
//     expression-list , expression
std::optional<CST::ExpressionList> Parser::parseExpressionList(SourceLocation openParenthesisLocation)
{
    auto parseParameterModifier = [this]() {
        if (current().lexeme == ParameterModifier::Out)
        {
            next();
            return ParameterModifier::Out;
        }
        else if (current().lexeme == ParameterModifier::Move)
        {
            next();
            return ParameterModifier::Move;
        }
        return ParameterModifier::In;
    };

    ParameterModifier parameterModifier{parseParameterModifier()};

    CST::ExpressionList expressions;
    expressions.openParenthesisLocation = openParenthesisLocation;

    // list is not empty
    if (CST::Node firstExpression = parseExpression())
    {
        expressions.add(CST::Node<CST::ExpressionTerm>{allocator(), std::move(firstExpression), parameterModifier});

        while (current().lexeme == Punctuator::Comma)
        {
            // skip the ,
            next();

            parameterModifier = parseParameterModifier();

            if (CST::Node nextExpression = parseExpression())
            {
                expressions.add(CST::Node<CST::ExpressionTerm>{allocator(), std::move(nextExpression), parameterModifier});
            }
            else
            {
                _diagnosis.error(DiagnosisMessage::invalidTextInExpressionList());
                return {};
            }
        }
    }

    expressions.closeParenthesisLocation = current().location;

    return expressions;
}

// prefix-expression:
//     prefix-operator prefix-expression
// TODO await-expression
// TODO sizeof ( type-id )
// TODO sizeof ... ( identifier )
// TODO alignof ( type-id )
// TODO throws-expression
CST::Node<CST::PrefixExpression> Parser::parsePrefixExpression()
{
    auto isPrefixToken = [](const Token& token) {
        const Lexeme lexeme = token.lexeme;
        return lexeme == Punctuator::Not || lexeme == Punctuator::Plus || lexeme == Punctuator::Minus;
    };

    CST::Node<CST::PrefixExpression> prefix{allocator()};

    for (; isPrefixToken(current()); next())
    {
        prefix->ops.emplace_back(current());
    }

    return prefix;
}

// postfix-expression:
//     postfix-operator
//     [ expression-list ]
//     ( expression-list? )
//     . id-expression
CST::Node<CST::PostfixExpression> Parser::parsePostfixExpression()
{
    CST::Node<CST::PostfixExpression> postfix{allocator()};

    // TODO Find a better name
    auto shouldContinue = [this]() {
        auto isLexicallyAdjacent = [](const Token& current, const Token& previous) {
            return current.location.line == previous.location.line && current.location.column == previous.location.column + static_cast<SourceColumn>(previous.text.size());
        };

        const Token& curr = current();
        const Token& previous = peekBack(1);

        // clang-format off
        const bool isPostfix = curr.lexeme  == Punctuator::PlusPlus ||
                                curr.lexeme  == Punctuator::MinusMinus ||
                                curr.lexeme  == Punctuator::Multiply ||
                                curr.lexeme  == Punctuator::Ampersand ||
                                curr.lexeme  == Punctuator::Tilde ||
                                curr.lexeme  == Punctuator::Dollar;

        if ((!isPostfix || !isLexicallyAdjacent(curr , previous)) &&
            (curr.lexeme != Punctuator::OpenBracket && curr.lexeme != Punctuator::OpenParenthesis && curr.lexeme != Punctuator::Dot))
        {
            return false;
        }

        //  * & and ~ can't be a unary operator if followed by ( or identifier
        const bool is_invalid_sequence =
            (curr.lexeme == Punctuator::Multiply || curr.lexeme == Punctuator::Ampersand || curr.lexeme == Punctuator::Tilde) &&
            (curr.lexeme == Punctuator::OpenParenthesis || curr.lexeme == Identifier{});
        // clang-format on

        return !is_invalid_sequence;
    };

    while (shouldContinue())
    {
        if (current().lexeme == Punctuator::Dollar)
        {
            // TODO handle $
            return {};
        }

        postfix->terms.emplace_back(current());
        CST::PostfixExpression::Term& term{postfix->terms.back()};

        next();

        auto parseExpressionListForTerm = [this, &term](
                                              Punctuator closeLexeme,
                                              auto errMsgExpressionsNullptrGetter,
                                              auto errMsgNotMatchCloseGetter) {
            term.expressions = parseExpressionList(term.op.get().location);

            if (term.expressions == std::nullopt)
            {
                _diagnosis.error(errMsgExpressionsNullptrGetter(), current().location);
                return false;
            }

            const Token& closeToken = current();

            if (closeToken.lexeme != closeLexeme)
            {
                _diagnosis.error(errMsgNotMatchCloseGetter(), current().location);
                return false;
            }

            // term.expressions.value().closeParenthesisLocation = closeToken.location;
            term.closeOp = closeToken;

            next();

            return true;
        };

        if (term.op.get().lexeme == Punctuator::OpenBracket)
        {
            if (!parseExpressionListForTerm(
                    Punctuator::CloseBracket,
                    &DiagnosisMessage::subscriptExpressionBracketEmpty,
                    &DiagnosisMessage::unexpectedTextBracketDoesNotMatch))
            {
                return {};
            }
        }
        else if (term.op.get().lexeme == Punctuator::OpenParenthesis)
        {
            if (!parseExpressionListForTerm(
                    Punctuator::CloseParenthesis,
                    &DiagnosisMessage::unexpectedTextAfterExpressionList,
                    &DiagnosisMessage::unexpectedTextParenthesisDoesNotMatch))
            {
                return {};
            }
        }
        else if (term.op.get().lexeme == Punctuator::Dot)
        {
            term.identifierExpression = parseIdentifierExpression();

            if (!term.identifierExpression)
            {
                _diagnosis.error(DiagnosisMessage::dotMustFollowedByValidMemberName(), term.op.get().location);
                return {};
            }
        }
    }

    return postfix;
}

CST::Node<CST::PrimaryExpression> Parser::parsePrimaryExpression()
{
    if (CST::Node id = parseIdentifierExpression())
    {
        CST::Node<CST::PrimaryExpression> node{allocator()};
        node->type = std::move(id);
        return node;
    }

    auto isLiteralOrIdentifierOrKeyword = [](const Token& token) {
        // clang-format off
        return token.lexeme.is<BinaryLiteral>() ||
               token.lexeme.is<BooleanLiteral>() ||
               token.lexeme.is<CharacterLiteral>() ||
               token.lexeme.is<DecimalLiteral>() ||
               token.lexeme.is<FloatingLiteral>() ||
               token.lexeme.is<Identifier>() ||
               token.lexeme.is<Keyword>() ||
               token.lexeme.is<PointerLiteral>() ||
               token.lexeme.is<StringLiteral>();
        // clang-format on
    };

    if (isLiteralOrIdentifierOrKeyword(current()))
    {
        CST::Node<CST::PrimaryExpression> node{allocator()};
        node->type = std::ref(current());
        next();
        return node;
    }

    if (current().lexeme == Punctuator::OpenParenthesis)
    {
        return parsePrimaryOpenParenthesisExpression();
    }

    if (CST::Node declaration = parseUnnamedDeclaration(current().location, true))
    {
        if (!declaration->type.is<CST::FunctionSignature>())
        {
            _diagnosis.error(DiagnosisMessage::unnamedDeclarationAtExpressionScopeMustBeFunction(), current().location);

            next();

            return {};
        }

        const CST::FunctionSignature& func = declaration->type.as<CST::FunctionSignature>();

        if (func.returns.is<CST::ParameterDeclarationList>())
        {
            _diagnosis.error(DiagnosisMessage::unnamedFunctionAtExpressionScopeCannotReturnsMultipleValues(), current().location);

            next();

            return {};
        }

        CST::Node<CST::PrimaryExpression> node{allocator()};
        node->type = std::move(declaration);
        return node;
    }

    return {};
}

CST::Node<CST::PrimaryExpression> Parser::parsePrimaryOpenParenthesisExpression()
{
    const SourceLocation openLocation = current().location;

    next();

    std::optional expressionsOpt = parseExpressionList(openLocation);

    if (!expressionsOpt.has_value())
    {
        _diagnosis.error(DiagnosisMessage::unexpectedTextAfterOpenParenthesis(), current().location);

        next();

        return {};
    }

    CST::ExpressionList& expressions = expressionsOpt.value();

    if (current().lexeme != Punctuator::CloseParenthesis)
    {
        _diagnosis.error(DiagnosisMessage::unexpectedTextAfterExpressionList(), current().location);

        next();

        return {};
    }

    expressions.closeParenthesisLocation = current().location;

    next();

    CST::Node<CST::PrimaryExpression> primary{allocator()};
    primary->type = CST::Node<CST::ExpressionList>{allocator(), std::move(expressions)};
    return primary;
}

// id-expression
//    unqualified-id
//    qualified-id
CST::Node<CST::IdentifierExpression> Parser::parseIdentifierExpression()
{
    if (CST::Node unqualifiedIdentifier = parseUnqualifiedIdentifier())
    {
        CST::Node<CST::IdentifierExpression> expression{allocator()};

        expression->location = current().location;
        expression->identifier.type = std::move(unqualifiedIdentifier);

        return expression;
    }
    return {};
}

CST::Node<CST::IterationStatement> Parser::parseIterationStatement()
{
    return {};
}

CST::Node<CST::FunctionSignature> Parser::parseFunctionSignature()
{
    CST::ParameterDeclarationList parameters;
    if (!parseParameterDeclarationList(parameters))
    {
        return {};
    }

    // Optional "throws"
    bool throws = false;
    if (current().lexeme == Keyword::Throws)
    {
        throws = true;

        next();
    }

    // Optional returns
    CST::FunctionSignature::Returns returns;
    if (current().lexeme == Punctuator::Arrow)
    {
        next();

        if (CST::Node id_expr = parseIdentifierExpression())
        {
            returns = std::move(id_expr);
        }
        else
        {
            CST::ParameterDeclarationList returns_parameters;
            if (parseParameterDeclarationList(returns_parameters))
            {
                // returns = std::move(returns_parameters);
            }
            else
            {
                _diagnosis.error(DiagnosisMessage::missingFunctionReturnAfterArrow(), current().location);
                return {};
            }
        }
    }

    CST::Node<CST::FunctionSignature> functionSignature{allocator()};
    functionSignature->parameters = std::move(parameters);
    functionSignature->returns = std::move(returns);
    functionSignature->throws = throws;
    return functionSignature;
}

template<typename BinaryExpressionT, typename... Args>
bool Parser::parseBinaryExpression(BinaryExpressionT& binaryExpression, auto parseExpressionMethod, auto isValidTokenFunctor, Args... args)
{
    if (!(this->*parseExpressionMethod)(binaryExpression, args...))
    {
        return false;
    }

    while (isValidTokenFunctor(current()))
    {
        // add the token
        binaryExpression.terms.add(CST::Node<typename BinaryExpressionT::Term>{allocator(), current()});

        // skip the operand
        next();

        // parse the expression
        if (!(this->*parseExpressionMethod)(binaryExpression.terms.back(), args...))
        {
            _diagnosis.error(DiagnosisMessage::invalidExpressionAfter(binaryExpression.terms.back().op.get().text), current().location);
            return false;
        }
    }

    return true;
}

// additive-expression:
//     multiplicative-expression
//     additive-expression + multiplicative-expression
//     additive-expression - multiplicative-expression
bool Parser::parseAdditiveExpression(CST::AdditiveExpression& expr)
{
    return parseBinaryExpression(expr, &Parser::parseMultiplicativeExpression, &anyTokenPunctuator<Punctuator::Plus, Punctuator::Minus>);
}

// basic-expression:
//     prefix-expression primary-expression postfix-expression
bool Parser::parseBasicExpression(CST::BasicExpression& expr)
{
    expr.prefix = parsePrefixExpression();
    expr.primary = parsePrimaryExpression();

    if (!expr.primary)
    {
        return false;
    }

    expr.postfix = parsePostfixExpression();

    return true;
}

// assignment-expression:
//     logical-or-expression
//     assignment-expression assignment-operator assignment-expression
bool Parser::parseAssignmentExpression(CST::AssignmentExpression& expr, bool allowRelationalComparison)
{
    // clang-format off
    return parseBinaryExpression(
        expr,
        &Parser::parseLogicalOrExpression,
        &anyTokenPunctuator<
            Punctuator::AmpersandEqual,
            Punctuator::Assignment,
            Punctuator::CaretEqual,
            Punctuator::LeftShiftEqual,
            Punctuator::MinusEqual,
            Punctuator::ModuloEqual,
            Punctuator::MultiplyEqual,
            Punctuator::PipeEqual,
            Punctuator::PlusEqual,
            Punctuator::RightShiftEqual,
            Punctuator::SlashEqual>,
        allowRelationalComparison);
    // clang-format on
}

// bit-and-expression:
//     equality-expression
//     bit-and-expression & equality-expression
bool Parser::parseBitAndExpression(CST::BitAndExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseEqualityExpression,
        &anyTokenPunctuator<Punctuator::Ampersand>,
        allowRelationalComparison);
}

// bit-or-expression:
//     bit-xor-expression
//     bit-or-expression & bit-xor-expression
bool Parser::parseBitOrExpression(CST::BitOrExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseBitXorExpression,
        &anyTokenPunctuator<Punctuator::Pipe>,
        allowRelationalComparison);
}

// bit-xor-expression:
//     bit-and-expression
//     bit-xor-expression & bit-and-expression
bool Parser::parseBitXorExpression(CST::BitXorExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseBitAndExpression,
        &anyTokenPunctuator<Punctuator::Caret>,
        allowRelationalComparison);
}

// compare-expression:
//    shift-expression
//    compare-expression <=> shift-expression
bool Parser::parseCompareExpression(CST::CompareExpression& expr)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseShiftExpression,
        &anyTokenPunctuator<Punctuator::Spaceship>);
}

// equality-expression:
//    relational-expression
//    equality-expression == relational-expression
//    equality-expression != relational-expression
bool Parser::parseEqualityExpression(CST::EqualityExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseRelationalExpression,
        &anyTokenPunctuator<Punctuator::CompareEqual, Punctuator::CompareNotEqual>,
        allowRelationalComparison);
}

// is-as-expression:
//     basic-expression
// TODO    is-as-expression is-expression-constraint
// TODO    is-as-expression as-type-cast
// TODO    type-id is-type-constraint
bool Parser::parseIsAsExpression(CST::IsAsExpression& expr)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseBasicExpression,
        &anyTokenKeyword<Keyword::As, Keyword::Is>);
}

// logical-and-expression:
//     bit-or-expression
//     logical-and-expression && bit-or-expression
bool Parser::parseLogicalAndExpression(CST::LogicalAndExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseBitOrExpression,
        &anyTokenPunctuator<Punctuator::LogicalAnd>,
        allowRelationalComparison);
}

// constant-expression:    // don't need intermediate production, just use:
// conditional-expression: // don't need intermediate production, just use:
// logical-or-expression:
//     logical-and-expression
//     logical-or-expression || logical-and-expression
bool Parser::parseLogicalOrExpression(CST::LogicalOrExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseLogicalAndExpression,
        &anyTokenPunctuator<Punctuator::LogicalOr>,
        allowRelationalComparison);
}

// multiplicative-expression:
//     is-as-expression
//     multiplicative-expression * is-as-expression
//     multiplicative-expression / is-as-expression
//     multiplicative-expression % is-as-expression
bool Parser::parseMultiplicativeExpression(CST::MultiplicativeExpression& expr)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseIsAsExpression,
        &anyTokenPunctuator<Punctuator::Multiply, Punctuator::Slash, Punctuator::Modulo>);
}

// relational-expression:
//     compare-expression
//     relational-expression <  compare-expression
//     relational-expression >  compare-expression
//     relational-expression <= compare-expression
//     relational-expression >= compare-expression
bool Parser::parseRelationalExpression(CST::RelationalExpression& expr, bool allowRelationalComparison)
{
    if (allowRelationalComparison)
    {
        return parseBinaryExpression(
            expr,
            &Parser::parseCompareExpression,
            &anyTokenPunctuator<Punctuator::Less, Punctuator::LessEqual, Punctuator::Greater, Punctuator::GreaterEqual>);
    }

    return parseBinaryExpression(
        expr,
        &Parser::parseCompareExpression,
        [](const Token&) { return false; });
}

// shift-expression:
//     additive-expression
//     shift-expression << additive-expression
//     shift-expression >> additive-expression
bool Parser::parseShiftExpression(CST::ShiftExpression& expr)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseAdditiveExpression,
        &anyTokenPunctuator<Punctuator::LeftShift, Punctuator::RightShift>);
}

// statement:
//     compound-statement
//     contract
//     declaration-statement
//     expression-statement
//     inspect-expression
//     iteration-statement
//     return-statement
//     selection-statement
//
// TODO     try-block
CST::Node<CST::Statement> Parser::parseStatement(bool mustEndWithSemicolon, SourceLocation equalLocation)
{
    auto makeStatementIfParseSucceed = [this]<typename... ArgsT>(auto parseStatementMethod, ArgsT&&... args) -> CST::Node<CST::Statement> {
        if (CST::Node stmtType = (this->*parseStatementMethod)(std::forward<ArgsT>(args)...))
        {
            CST::Node<CST::Statement> stmt{allocator()};
            stmt->type = std::move(stmtType);
            return stmt;
        }
        return {};
    };

    if (CST::Node stmt = makeStatementIfParseSucceed(&Parser::parseSelectionStatement)) return stmt;
    if (CST::Node stmt = makeStatementIfParseSucceed(&Parser::parseReturnStatement)) return stmt;
    if (CST::Node stmt = makeStatementIfParseSucceed(&Parser::parseIterationStatement)) return stmt;
    if (CST::Node stmt = makeStatementIfParseSucceed(&Parser::parseCompoundStatement, equalLocation)) return stmt;
    if (CST::Node stmt = makeStatementIfParseSucceed(&Parser::parseDeclaration, mustEndWithSemicolon)) return stmt;
    if (CST::Node stmt = makeStatementIfParseSucceed(&Parser::parseExpressionStatement, mustEndWithSemicolon)) return stmt;

    return {};
}

// compound-statement:
//     { statement-seq-opt }
//
// statement-seq:
//     statement
//     statement-seq statement
CST::Node<CST::CompoundStatement> Parser::parseCompoundStatement(SourceLocation equalLocation)
{
    const Token& openBraceToken = current();

    if (openBraceToken.lexeme != Punctuator::OpenBrace)
    {
        return {};
    }

    const SourceLocation openBraceLocation = equalLocation.line == openBraceToken.location.line ? equalLocation : openBraceToken.location;

    // slip the {
    next();

    CST::Node<CST::CompoundStatement> statements{allocator()};

    statements->openBraceLocation = openBraceLocation;

    while (current().lexeme != Punctuator::CloseBrace)
    {
        static constexpr bool mustEndWithSemicolon = true;
        if (CST::Node statement = parseStatement(mustEndWithSemicolon))
        {
            statements->add(std::move(statement));
        }
        else
        {
            _diagnosis.error(DiagnosisMessage::invalidStatementInCompoundStatement(), current().location);
            return {};
        }
    }

    statements->closeBraceLocation = current().location;

    // skip the }
    next();

    return statements;
}

// expression-statement:
//    expression ;
//    expression
CST::Node<CST::ExpressionStatement> Parser::parseExpressionStatement(bool mustEndWithSemicolon)
{
    CST::Node expression = parseExpression();

    if (!expression)
    {
        return {};
    }

    bool hasSemicolon = false;

    if (current().lexeme == Punctuator::Semicolon)
    {
        hasSemicolon = true;

        // skip the ;
        next();
    }
    else if (mustEndWithSemicolon)
    {
        _diagnosis.error(DiagnosisMessage::missingSemicolonAtEndStatement(), current().location);
        return {};
    }

    CST::Node<CST::ExpressionStatement> exprStmt{allocator()};
    exprStmt->expression = std::move(expression);
    exprStmt->hasSemicolon = hasSemicolon;
    return exprStmt;
}

// return-statement:
//     return expression-opt ;
CST::Node<CST::ReturnStatement> Parser::parseReturnStatement()
{
    const Token& identifier = current();

    if (identifier.lexeme != Keyword::Return)
    {
        return {};
    }

    next();

    if (current().lexeme == Punctuator::Semicolon)
    {
        next();

        CST::Node<CST::ReturnStatement> returnStatement{allocator(), identifier};
        return returnStatement;
    }

    CST::Node expression = parseExpression();

    if (!expression)
    {
        _diagnosis.error(DiagnosisMessage::invalidReturnExpression(), current().location);
        return {};
    }

    if (current().lexeme != Punctuator::Semicolon)
    {
        _diagnosis.error(DiagnosisMessage::missingSemicolonAtEndStatement(), current().location);

        next();

        return {};
    }

    next();

    CST::Node<CST::ReturnStatement> returnStatement{allocator(), identifier};
    returnStatement->expression = std::move(expression);
    return returnStatement;
}

// selection-statement:
//     if constexpr-opt expression compound-statement
//     if constexpr-opt expression compound-statement else compound-statement
CST::Node<CST::SelectionStatement> Parser::parseSelectionStatement()
{
    return {};
}

// unqualified-id:
//    const-opt identifier
//    const-opt template-id
// TODO operator-function-id
//
// template-id:
//    identifier < template-argument-list-opt >
//
// template-argument-list:
//    template-argument-list , template-argument
//
// template-argument:
//    expression
//    id-expression
//
CST::Node<CST::UnqualifiedIdentifier> Parser::parseUnqualifiedIdentifier()
{
    if (!current().lexeme.is<Identifier>() && !current().lexeme.is<Keyword>())
    {
        return {};
    }

    CST::Node<CST::UnqualifiedIdentifier> identifier{allocator(), current()};

    next();

    if (current().lexeme == Punctuator::Less)
    {
        // TODO parse template arguments
        return {};
    }

    return identifier;
}

bool Parser::isEnd() const
{
    return _currentTokenIndex >= _tokens.size();
}

const Token& Parser::current() const
{
    return !isEnd() ? _tokens.at(_currentTokenIndex) : Token::invalid();
}

const Token& Parser::peekBack(std::size_t offset) const
{
    assert(_currentTokenIndex >= offset);

    const std::size_t i = _currentTokenIndex - offset;

    assert(i < _tokens.size());

    return _tokens.at(i);
}

void Parser::next()
{
    ++_currentTokenIndex;
}

CST::TranslationUnit::Allocator& Parser::allocator()
{
    return _tu.allocator;
}

} // namespace CPPS
