#pragma once

#include <string>
#include <vector>

#include "cpps/comment.hpp"
#include "cpps/token.hpp"

namespace CPPS {

class Diagnosis;
class Source;
class Tokens;

struct SourceLocation;

class Lexer
{
public:
    struct DiagnosisMessage
    {
        static std::string binaryLiteralInvalidFormat();
        static std::string characterLiteralEmpty();
        static std::string characterLiteralMissingClosingQuote();
        static std::string floatingLiteralInvalidFormat(std::string_view text);
        static std::string hexadecimalLiteralInvalidFormat();
        static std::string stringLiteralMissingClosingQuote();
        static std::string unexpectedCharacter(char c);
        static std::string universalCharacterNameInvalidFormat(char c);
    };

public:
    Lexer(Diagnosis& diagnosis, const Source& source);

    [[nodiscard]] Tokens lex();

private:
    void lexLine();

    void lexInComment();
    void lexOutComment();

    void lexAmpersand();
    void lexAssignment();
    void lexCaret();
    void lexCloseBrace();
    void lexCloseBracket();
    void lexCloseParenthesis();
    void lexColon();
    void lexComma();
    void lexDollar();
    void lexDot();
    void lexGreater();
    void lexLess();
    void lexMinus();
    void lexModulo();
    void lexMultiply();
    void lexNot();
    void lexOpenBrace();
    void lexOpenBracket();
    void lexOpenParenthesis();
    void lexPipe();
    void lexPlus();
    void lexQuestionMark();
    void lexSemicolon();
    void lexSlash();
    void lexTilde();

    [[nodiscard]] bool tryLexCharacterLiteral();
    [[nodiscard]] bool tryLexNumberLiteral();
    [[nodiscard]] bool tryLexStringLiteral();

    [[nodiscard]] bool tryLexKeyword();
    [[nodiscard]] bool tryLexIdentifier();

    template<typename T>
    [[nodiscard]] bool tryLexKeywordContainer(auto& container, auto isValidChar);

    [[nodiscard]] std::size_t findQuoteBeginOffset(char quote) const;

    [[nodiscard]] std::size_t findCharacterEndIndex(std::size_t offset, char quote);
    [[nodiscard]] std::size_t findUniversalCharacterEndIndex(std::size_t startOffset);
    [[nodiscard]] std::size_t findEscapeSequenceEndIndex(std::size_t offset) const;
    [[nodiscard]] std::size_t findBasicCharacterEndIndex(std::size_t offset, char quote) const;

    [[nodiscard]] bool validateQuotedLiteralEndIndex(std::size_t endIndex, char quote);

private:
    void next();
    [[nodiscard]] char current() const;
    [[nodiscard]] char peek(std::size_t offset) const;

    [[nodiscard]] SourceLocation currentLocation() const;

private:
    void addToken(std::size_t size, Lexeme lexeme);
    void addToken(std::size_t size, Punctuator lexeme);

    void addTernaryConditionalToken(char conditionCharacter, Lexeme trueValue, Lexeme falseValue);
    void addTernaryConditionalToken(char conditionCharacter, Punctuator trueValue, Punctuator falseValue);

private:
    void addComment(std::string_view text, SourceLocation beginLocation, SourceLocation endLocation);
    void beginComment();
    void endComment();

private:
    Diagnosis& _diagnosis;
    const Source& _source;

    std::vector<Token> _tokens;
    std::vector<Comment> _comments;

    std::string_view _currentLine;
    std::size_t _currentLineIndex{0};
    std::size_t _currentColumnIndex{0};

    bool _isInComment{false};
};

} // namespace CPPS
