#include "cpps/lexer.hpp"

#include "cpps/check-diagnosis.hpp"
#include "cpps/diagnosis.hpp"
#include "cpps/source.hpp"
#include "cpps/tokens.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

namespace CPPS {

struct CheckLine
{
    std::string text;
    std::vector<Token> expectedTokens;
};

struct CheckErrorLine
{
    std::string text;
    SourceLocation expectedDiagnosisLocation;
};

Token token(auto lexeme, SourceLine line, SourceColumn column, std::string_view text)
{
    return {Lexeme{lexeme}, SourceLocation{line, column}, text};
}

void check(auto lexeme, std::string_view base)
{
    INFO(fmt::format("based on {}", base));

    const std::array lines = std::to_array<CheckLine>(
        {{std::string(base), {token(lexeme, 0, 0, base)}},
         {fmt::format(";{}", base), {token(Lexeme::Semicolon, 1, 0, ";"), token(lexeme, 1, 1, base)}},
         {fmt::format("{};", base), {token(lexeme, 2, 0, base), token(Lexeme::Semicolon, 2, static_cast<SourceColumn>(base.size()), ";")}},
         {fmt::format("; {}", base), {token(Lexeme::Semicolon, 3, 0, ";"), token(lexeme, 3, 2, base)}},
         {fmt::format("{} ;", base), {token(lexeme, 4, 0, base), token(Lexeme::Semicolon, 4, static_cast<SourceColumn>(base.size() + 1U), ";")}},
         {fmt::format(";{};", base), {token(Lexeme::Semicolon, 5, 0, ";"), token(lexeme, 5, 1, base), token(Lexeme::Semicolon, 5, static_cast<SourceColumn>(base.size() + 1U), ";")}},
         {fmt::format("; {} ;", base), {token(Lexeme::Semicolon, 6, 0, ";"), token(lexeme, 6, 2, base), token(Lexeme::Semicolon, 6, static_cast<SourceColumn>(base.size() + 3U), ";")}}});

    Diagnosis diagnosis;
    Source source;
    Lexer lexer{diagnosis, source};

    std::size_t tokensCount{0};

    for (const CheckLine& line : lines)
    {
        source.add(line.text, Source::Line::Type::Cpps);

        tokensCount += line.expectedTokens.size();
    }

    const Tokens tokens{lexer.lex()};

    checkNoErrorOrWarning(diagnosis);

    REQUIRE(tokens.lines() == lines.size());
    CHECK(tokens.size() == tokensCount);

    for (std::size_t l = 0; l < lines.size(); ++l)
    {
        INFO(fmt::format("Line {}", l));

        const CheckLine& line = lines[l];

        REQUIRE(tokens.on(l).size() == line.expectedTokens.size());

        for (std::size_t t = 0; t < line.expectedTokens.size(); ++t)
        {
            CHECK(tokens.at(l, t) == line.expectedTokens[t]);
        }
    }
}

void checkErrors(std::vector<std::string> expectedDiagnosisMessages, std::string_view base)
{
    INFO(fmt::format("based on {}", base));

    const std::array lines = std::to_array<CheckErrorLine>(
        {{std::string(base), SourceLocation{0, 0}},
         {fmt::format(";{}", base), SourceLocation{1, 1}},
         {fmt::format("{};", base), SourceLocation{2, 0}},
         {fmt::format("; {}", base), SourceLocation{3, 2}},
         {fmt::format("{} ;", base), SourceLocation{4, 0}},
         {fmt::format(";{};", base), SourceLocation{5, 1}},
         {fmt::format("; {} ;", base), SourceLocation{6, 2}}});

    Diagnosis diagnosis;
    Source source;
    Lexer lexer{diagnosis, source};

    for (const CheckErrorLine& line : lines)
    {
        source.add(line.text, Source::Line::Type::Cpps);
    }

    const Tokens tokens{lexer.lex()};

    checkNoWarning(diagnosis);

    const std::span errors = diagnosis.getErrors();

    REQUIRE(errors.size() == (tokens.lines() * expectedDiagnosisMessages.size()));

    for (std::size_t l = 0; l < lines.size(); ++l)
    {
        INFO(fmt::format("Line {}", l));

        for (std::size_t i = 0; i < expectedDiagnosisMessages.size(); ++i)
        {
            const std::size_t e = (l * expectedDiagnosisMessages.size()) + i;

            REQUIRE(errors[e].location);

            CHECK(errors[e].message == expectedDiagnosisMessages[i]);
            CHECK(errors[e].location->line == lines[l].expectedDiagnosisLocation.line);
        }
    }
}

void checkError(std::string expectedDiagnosisMessage, std::string_view base)
{
    checkErrors(std::vector{std::move(expectedDiagnosisMessage)}, base);
}

template<typename T>
requires(std::is_enum_v<T>)
void checkType(std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        const T value = static_cast<T>(i);

        check(value, toStringView(value));
    }
}

TEST_CASE("Lexer BinaryLiteral", "[Lexer]")
{
    check(Lexeme::BinaryLiteral, "0b01");
    check(Lexeme::BinaryLiteral, "0B01");
    check(Lexeme::BinaryLiteral, "0b0'1");
    check(Lexeme::BinaryLiteral, "0B0'1");
}

TEST_CASE("Lexer BooleanLiteral", "[Lexer]")
{
    checkType<BooleanLiteral>(BooleanLiteralCount);
}

TEST_CASE("Lexer CharacterLiteral", "[Lexer]")
{
    // basic character
    check(Lexeme::CharacterLiteral, R"('a')");

    // escape sequence character
    check(Lexeme::CharacterLiteral, R"('\n')");
    check(Lexeme::CharacterLiteral, R"('\x01')");

    // universal character name
    check(Lexeme::CharacterLiteral, R"('\u01AF')");
    check(Lexeme::CharacterLiteral, R"('\U00000041')");

    // encoding-prefix
    check(Lexeme::CharacterLiteral, R"(u'a')");
    check(Lexeme::CharacterLiteral, R"(u8'a')");

    // quote
    check(Lexeme::CharacterLiteral, R"('\'')");
}

TEST_CASE("Lexer DecimalLiteral", "[Lexer]")
{
    check(Lexeme::DecimalLiteral, "012345679");
    check(Lexeme::DecimalLiteral, "012'345'679");
}

TEST_CASE("Lexer FloatingLiteral", "[Lexer]")
{
    check(Lexeme::FloatingLiteral, "012345.679");
    check(Lexeme::FloatingLiteral, "012'345.679");
}

TEST_CASE("Lexer FunctionModifier", "[Lexer]")
{
    checkType<FunctionModifier>(FunctionModifierCount);
}

TEST_CASE("Lexer HexadecimalLiteral", "[Lexer]")
{
    check(Lexeme::HexadecimalLiteral, "0x0f");
    check(Lexeme::HexadecimalLiteral, "0x0F");
    check(Lexeme::HexadecimalLiteral, "0x0'f");
    check(Lexeme::HexadecimalLiteral, "0x0'F");
    check(Lexeme::HexadecimalLiteral, "0x0ab1022f");
    check(Lexeme::HexadecimalLiteral, "0x0ab1022F");
}

TEST_CASE("Lexer Keyword", "[Lexer]")
{
    checkType<Keyword>(KeywordCount);
}

TEST_CASE("Lexer Lexeme::Basic ", "[Lexer]")
{
    for (std::size_t i = 0; i < Lexeme::BasicCount; ++i)
    {
        const Lexeme::Basic basic = static_cast<Lexeme::Basic>(i);
        const std::string_view basicSv = Lexeme::BasicStrings[basic];

        if (!basicSv.empty())
        {
            check(basic, basicSv);
        }
    }
}

TEST_CASE("Lexer ParameterModifier", "[Lexer]")
{
    checkType<ParameterModifier>(ParameterModifierCount);
}

TEST_CASE("Lexer PointerLiteral", "[Lexer]")
{
    checkType<PointerLiteral>(PointerLiteralCount);
}

TEST_CASE("Lexer StringLiteral", "[Lexer]")
{
    // basic character
    check(Lexeme::StringLiteral, R"("a string")");

    // escape sequence character
    check(Lexeme::StringLiteral, R"("a \n string")");
    check(Lexeme::StringLiteral, R"("a \x01 string")");

    // universal character name
    check(Lexeme::StringLiteral, R"("a \u01AF string")");
    check(Lexeme::StringLiteral, R"("a \U00000041 string")");

    // encoding-prefix
    check(Lexeme::StringLiteral, R"(u"a string")");
    check(Lexeme::StringLiteral, R"(u8"a string")");

    // quote
    check(Lexeme::StringLiteral, R"("\"")");
    check(Lexeme::StringLiteral, R"("a \"quoted\" string")");
}

TEST_CASE("Lexer Comment", "[Lexer]")
{
    Diagnosis diagnosis;
    Source source;
    Lexer lexer{diagnosis, source};

    SECTION("Type::Line")
    {
        static constexpr std::string_view CommentText = "// comment";

        source.add(std::string(CommentText), Source::Line::Type::Cpps);

        const Tokens tokens = lexer.lex();
        const std::span comments = tokens.comments();

        REQUIRE(comments.size() == 1);

        CHECK(comments[0].type == Comment::Type::Line);
        CHECK(comments[0].text == CommentText);
        CHECK(comments[0].beginLocation == SourceLocation{0, 0});
        CHECK(comments[0].endLocation == SourceLocation{0, 9});
    }

    SECTION("Type::Block")
    {
        SECTION("on a single line")
        {
            static constexpr std::string_view CommentText = "/* comment */";

            source.add(std::string(CommentText), Source::Line::Type::Cpps);

            const Tokens tokens = lexer.lex();
            const std::span comments = tokens.comments();

            REQUIRE(comments.size() == 1);

            CHECK(comments[0].type == Comment::Type::Block);
            CHECK(comments[0].text == CommentText);
            CHECK(comments[0].beginLocation == SourceLocation{0, 0});
            CHECK(comments[0].endLocation == SourceLocation{0, 12});
        }

        SECTION("on multiple line")
        {
            static constexpr std::string_view CommentTextLine0 = "/* comment";
            static constexpr std::string_view CommentTextLine1 = " */";
            static const std::string CommentText = std::string(CommentTextLine0) + std::string(CommentTextLine1);

            source.add(std::string(CommentTextLine0), Source::Line::Type::Cpps);
            source.add(std::string(CommentTextLine1), Source::Line::Type::Cpps);

            const Tokens tokens = lexer.lex();
            const std::span comments = tokens.comments();

            REQUIRE(comments.size() == 1);

            CHECK(comments[0].type == Comment::Type::Block);
            CHECK(comments[0].text == CommentText);
            CHECK(comments[0].beginLocation == SourceLocation{0, 0});
            CHECK(comments[0].endLocation == SourceLocation{1, 2});
        }
    }
}

TEST_CASE("Lexer sequence of tokens", "[Lexer]")
{
    Diagnosis diagnosis;
    Source source;
    Lexer lexer{diagnosis, source};

    source.add("int main() // main function", Source::Line::Type::Cpps);
    source.add("{", Source::Line::Type::Cpps);
    source.add("    if (true) /* dummy condition */", Source::Line::Type::Cpps);
    source.add(" /* fancy */return 1;", Source::Line::Type::Cpps);
    source.add("    return 0;", Source::Line::Type::Cpps);
    source.add("}", Source::Line::Type::Cpps);

    const Tokens tokens = lexer.lex();
    const std::span comments = tokens.comments();

    REQUIRE(tokens.lines() == 6);
    REQUIRE(tokens.size() == 16);
    REQUIRE(comments.size() == 3);

    CHECK(tokens.at(0, 0) == token(Keyword::Int, 0, 0, "int"));
    CHECK(tokens.at(0, 1) == token(Lexeme::Identifier, 0, 4, "main"));
    CHECK(tokens.at(0, 2) == token(Lexeme::OpenParenthesis, 0, 8, "("));
    CHECK(tokens.at(0, 3) == token(Lexeme::CloseParenthesis, 0, 9, ")"));
    CHECK(tokens.at(1, 0) == token(Lexeme::OpenBrace, 1, 0, "{"));
    CHECK(tokens.at(2, 0) == token(Keyword::If, 2, 4, "if"));
    CHECK(tokens.at(2, 1) == token(Lexeme::OpenParenthesis, 2, 7, "("));
    CHECK(tokens.at(2, 2) == token(BooleanLiteral::True, 2, 8, "true"));
    CHECK(tokens.at(2, 3) == token(Lexeme::CloseParenthesis, 2, 12, ")"));
    CHECK(tokens.at(3, 0) == token(Keyword::Return, 3, 12, "return"));
    CHECK(tokens.at(3, 1) == token(Lexeme::DecimalLiteral, 3, 19, "1"));
    CHECK(tokens.at(3, 2) == token(Lexeme::Semicolon, 3, 20, ";"));
    CHECK(tokens.at(4, 0) == token(Keyword::Return, 4, 4, "return"));
    CHECK(tokens.at(4, 1) == token(Lexeme::DecimalLiteral, 4, 11, "0"));
    CHECK(tokens.at(4, 2) == token(Lexeme::Semicolon, 4, 12, ";"));
    CHECK(tokens.at(5, 0) == token(Lexeme::CloseBrace, 5, 0, "}"));
}

TEST_CASE("Lexer DiagnosisMessage", "[Lexer]")
{
    SECTION("binaryLiteralInvalidFormat")
    {
        checkError(Lexer::DiagnosisMessage::binaryLiteralInvalidFormat(), "0b2");
    }

    SECTION("characterLiteralEmpty")
    {
        checkError(Lexer::DiagnosisMessage::characterLiteralEmpty(), "''");
    }

    SECTION("characterLiteralMissingClosingQuote")
    {
        checkError(Lexer::DiagnosisMessage::characterLiteralMissingClosingQuote(), "'a");
        checkError(Lexer::DiagnosisMessage::characterLiteralMissingClosingQuote(), "'a;");
    }

    SECTION("floatingLiteralInvalidFormat")
    {
        checkError(Lexer::DiagnosisMessage::floatingLiteralInvalidFormat("0."), "0.Z");
    }

    SECTION("hexadecimalLiteralInvalidFormat")
    {
        checkError(Lexer::DiagnosisMessage::hexadecimalLiteralInvalidFormat(), "0xZ");
    }

    SECTION("stringLiteralMissingClosingQuote")
    {
        checkError(Lexer::DiagnosisMessage::stringLiteralMissingClosingQuote(), R"("a string)");
    }

    SECTION("unexpectedCharacter")
    {
        checkError(Lexer::DiagnosisMessage::unexpectedCharacter('@'), "@");
    }

    SECTION("universalCharacterNameInvalidFormat")
    {
        // clang-format off
        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('u'), R"('\u0')");     // too small
        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('u'), R"('\u00000')"); // too big
        checkErrors({Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('u'),
                     Lexer::DiagnosisMessage::characterLiteralMissingClosingQuote()}, R"('\u000Z')"); // invalid char

        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('U'), R"('\U0')");         // too small
        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('U'), R"('\U000000000')"); // too big
        checkErrors({Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('U'),
                     Lexer::DiagnosisMessage::characterLiteralMissingClosingQuote()}, R"('\U0000000Z')"); // invalid char

        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('u'), R"("\u0")");     // too small
        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('u'), R"("\u00000")"); // too big
        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('u'), R"("\u000Z")");  // invalid char

        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('U'), R"("\U0")");         // too small
        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('U'), R"("\U000000000")"); // too big
        checkError(Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat('U'), R"("\U0000000Z")");  // invalid char
        // clang-format on
    }
}

TEST_CASE("Lexer source is empty", "[Lexer]")
{
    Diagnosis diagnosis;
    Source source;
    Lexer lexer{diagnosis, source};

    const Tokens tokens = lexer.lex();

    checkNoErrorOrWarning(diagnosis);

    CHECK(tokens.empty());
}

} // namespace CPPS
