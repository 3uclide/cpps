#include "cpps/lexer.hpp"

#include "cpps/diagnosis.hpp"
#include "cpps/source.hpp"
#include "cpps/tokens.hpp"
#include "cpps/utility.hpp"

#include <fmt/format.h>

namespace CPPS {

std::string Lexer::DiagnosisMessage::binaryLiteralInvalidFormat()
{
    return "binary literal cannot be empty, 0b (or 0B) must be followed by binary digits";
}

std::string Lexer::DiagnosisMessage::characterLiteralEmpty()
{
    return "empty character constant";
}

std::string Lexer::DiagnosisMessage::characterLiteralMissingClosingQuote()
{
    return "character literal {} is missing its closing";
}

std::string Lexer::DiagnosisMessage::floatingLiteralInvalidFormat(std::string_view text)
{
    return fmt::format("floating point literal {} fractional part cannot be empty (if floating point was intended, use .0)", text);
}

std::string Lexer::DiagnosisMessage::hexadecimalLiteralInvalidFormat()
{
    return "hexadecimal literal cannot be empty, 0x (or 0X) must be followed by hexadecimal digits";
}

std::string Lexer::DiagnosisMessage::stringLiteralMissingClosingQuote()
{
    return "string literal {} is missing its closing";
}

std::string Lexer::DiagnosisMessage::unexpectedCharacter(char c)
{
    return fmt::format("unexpected text '{}'", c);
}

std::string Lexer::DiagnosisMessage::universalCharacterNameInvalidFormat(char c, std::size_t size)
{
    return fmt::format("invalid universal character name (\\{} must be followed by {} hexadecimal digits)", c, size);
}

Lexer::Lexer(Diagnosis& diagnosis, const Source& source)
    : _diagnosis(diagnosis)
    , _source(source)
{
}

Tokens Lexer::lex()
{
    if (!_source.hasCpps())
    {
        return {};
    }

    for (_currentLineIndex = 0; _currentLineIndex < _source.size(); ++_currentLineIndex)
    {
        const Source::Line& line = _source[static_cast<SourceLine>(_currentLineIndex)];

        if (line.getType() == Source::Line::Type::Cpps)
        {
            _currentLine = line.getText();

            // skip spaces
            _currentColumnIndex = _isInComment ? 0 : findFirstNotOf(_currentLine, isSpace);

            lexLine();
        }
    }

    assert(!_isInComment);

    return Tokens{std::move(_tokens), std::move(_comments)};
}

void Lexer::lexLine()
{
    for (; _currentColumnIndex < _currentLine.size(); next())
    {
        if (_isInComment)
        {
            lexInComment();
        }
        else
        {
            lexOutComment();
        }
    }
}

void Lexer::lexInComment()
{
    if (current() == '*' && peek(1) == '/')
    {
        _comments.back().text += "*/";

        next();

        endComment();
    }
    else
    {
        _comments.back().text += current();
    }
}

void Lexer::lexOutComment()
{
    switch (current())
    {
    case '&': lexAmpersand(); break;
    case '=': lexAssignment(); break;
    case '^': lexCaret(); break;
    case '}': lexCloseBrace(); break;
    case ']': lexCloseBracket(); break;
    case ')': lexCloseParenthesis(); break;
    case ':': lexColon(); break;
    case ',': lexComma(); break;
    case '$': lexDollar(); break;
    case '.': lexDot(); break;
    case '>': lexGreater(); break;
    case '<': lexLess(); break;
    case '-': lexMinus(); break;
    case '%': lexModulo(); break;
    case '*': lexMultiply(); break;
    case '!': lexNot(); break;
    case '{': lexOpenBrace(); break;
    case '[': lexOpenBracket(); break;
    case '(': lexOpenParenthesis(); break;
    case '|': lexPipe(); break;
    case '+': lexPlus(); break;
    case '?': lexQuestionMark(); break;
    case ';': lexSemicolon(); break;
    case '/': lexSlash(); break;
    case '~': lexTilde(); break;
    default:
        // clang-format off
        if (!tryLexCharacterLiteral() &&
            !tryLexNumberLiteral() &&
            !tryLexStringLiteral() &&
            !tryLexKeyword() &&
            !tryLexIdentifier() &&

            !isSpace(current())) // anything else should be a whitespace
        {
            _diagnosis.error(DiagnosisMessage::unexpectedCharacter(current()), currentLocation());
        }
        // clang-format on
        break;
    }
}

// &&= && &= &
void Lexer::lexAmpersand()
{
    if (peek(1) == '&')
    {
        if (peek(2) == '=')
        {
            addToken(3, Lexeme::LogicalAndEqual);
        }
        else
        {
            addToken(2, Lexeme::LogicalAnd);
        }
    }
    else
    {
        addTernaryConditionalToken('=', Lexeme::AmpersandEqual, Lexeme::Ampersand);
    }
}

// == =
void Lexer::lexAssignment()
{
    addTernaryConditionalToken('=', Lexeme::CompareEqual, Lexeme::Assignment);
}

// ^= ^
void Lexer::lexCaret()
{
    addTernaryConditionalToken('=', Lexeme::CaretEqual, Lexeme::Caret);
}

// }
void Lexer::lexCloseBrace()
{
    addToken(1, Lexeme::CloseBrace);
}

// ]
void Lexer::lexCloseBracket()
{
    addToken(1, Lexeme::CloseBracket);
}

// )
void Lexer::lexCloseParenthesis()
{
    addToken(1, Lexeme::CloseParenthesis);
}

// :: :
void Lexer::lexColon()
{
    addTernaryConditionalToken(':', Lexeme::DoubleColon, Lexeme::Colon);
}

// ,
void Lexer::lexComma()
{
    addToken(1, Lexeme::Comma);
}

// $
void Lexer::lexDollar()
{
    addToken(1, Lexeme::Dollar);
}

// ... .
void Lexer::lexDot()
{
    if (peek(1) == '.' && peek(2) == '.')
    {
        addToken(3, Lexeme::Ellipsis);
    }
    else
    {
        addToken(1, Lexeme::Dot);
    }
}

// >>= >> >= >
void Lexer::lexGreater()
{
    if (peek(1) == '>')
    {
        if (peek(2) == '=')
        {
            addToken(3, Lexeme::RightShiftEqual);
        }
        else
        {
            addToken(2, Lexeme::RightShift);
        }
    }
    else
    {
        addTernaryConditionalToken('=', Lexeme::GreaterEqual, Lexeme::Greater);
    }
}

// <<= << <=> <= <
void Lexer::lexLess()
{
    switch (peek(1))
    {
    case '<':
        if (peek(2) == '=')
        {
            addToken(3, Lexeme::LeftShiftEqual);
        }
        else
        {
            addToken(2, Lexeme::LeftShift);
        }
        break;

    case '=':
        if (peek(2) == '>')
        {
            addToken(3, Lexeme::Spaceship);
        }
        else
        {
            addToken(2, Lexeme::LessEqual);
        }
        break;

    default:
        addToken(1, Lexeme::Less);
        break;
    }
}

// -- -= -> -
void Lexer::lexMinus()
{
    switch (peek(1))
    {
    case '-':
        addToken(2, Lexeme::MinusMinus);
        break;

    case '=':
        addToken(2, Lexeme::MinusEqual);
        break;

    case '>':
        addToken(2, Lexeme::Arrow);
        break;

    default:
        addToken(1, Lexeme::Minus);
        break;
    }
}

// %= %
void Lexer::lexModulo()
{
    addTernaryConditionalToken('=', Lexeme::ModuloEqual, Lexeme::Modulo);
}

// *= *
void Lexer::lexMultiply()
{
    addTernaryConditionalToken('=', Lexeme::MultiplyEqual, Lexeme::Multiply);
}

// != !
void Lexer::lexNot()
{
    addTernaryConditionalToken('=', Lexeme::CompareNotEqual, Lexeme::Not);
}

// {
void Lexer::lexOpenBrace()
{
    addToken(1, Lexeme::OpenBrace);
}

// [
void Lexer::lexOpenBracket()
{
    addToken(1, Lexeme::OpenBracket);
}

// (
void Lexer::lexOpenParenthesis()
{
    addToken(1, Lexeme::OpenParenthesis);
}

// ||= || |= |
void Lexer::lexPipe()
{
    if (peek(1) == '|')
    {
        if (peek(2) == '=')
        {
            addToken(3, Lexeme::LogicalOrEqual);
        }
        else
        {
            addToken(2, Lexeme::LogicalOr);
        }
    }
    else
    {
        addTernaryConditionalToken('=', Lexeme::PipeEqual, Lexeme::Pipe);
    }
}

// ++ += +
void Lexer::lexPlus()
{
    if (peek(1) == '+')
    {
        addToken(2, Lexeme::PlusPlus);
    }
    else
    {
        addTernaryConditionalToken('=', Lexeme::PlusEqual, Lexeme::Plus);
    }
}

void Lexer::lexQuestionMark()
{
    addToken(1, Lexeme::QuestionMark);
}

// ;
void Lexer::lexSemicolon()
{
    addToken(1, Lexeme::Semicolon);
}

// /* // /= /
void Lexer::lexSlash()
{
    switch (peek(1))
    {
    case '*':
        beginComment();
        next();
        break;

    case '/':
        addComment(_currentLine.substr(_currentColumnIndex),
                   currentLocation(),
                   SourceLocation{.line = static_cast<SourceLine>(_currentLineIndex),
                                  .column = static_cast<SourceColumn>(_currentLine.size() - 1U)});

        // end the current line, it's a comment line
        _currentColumnIndex = _currentLine.size();
        break;

    default:
        addTernaryConditionalToken('=', Lexeme::SlashEqual, Lexeme::Slash);
        break;
    }
}

// ~= ~
void Lexer::lexTilde()
{
    addTernaryConditionalToken('=', Lexeme::TildeEqual, Lexeme::Tilde);
}

// { encoding-prefix }? ' { c-char }* '
bool Lexer::tryLexCharacterLiteral()
{
    static constexpr char Quote = '\'';

    const std::size_t offset = findQuoteBeginOffset(Quote);

    if (offset == std::string_view::npos)
    {
        return false;
    }

    // empty char
    if (peek(offset) == '\'')
    {
        _diagnosis.error(DiagnosisMessage::characterLiteralEmpty(), currentLocation());

        // skip the '
        next();

        return true;
    }

    std::size_t endIndex = findCharacterEndIndex(offset, Quote);

    if (endIndex != std::string_view::npos)
    {
        ++endIndex;
    }

    if (!validateQuotedLiteralEndIndex(endIndex, Quote))
    {
        endIndex = _currentLine.size();
    }

    addToken(endIndex - _currentColumnIndex + 1, Lexeme::CharacterLiteral);

    return true;
}

// floating-point-literal   { ' | digit }* . { ' | digit }*
// integer-literal
//   binary-literal         { ' | binary-digit }*
//   decimal-literal        { ' | digit }*
//   hexadecimal-literal    { ' | hexadecimal-digit }*
bool Lexer::tryLexNumberLiteral()
{
    auto findFirstNotLiteral = [this](auto predicate, std::size_t offset) {
        const std::size_t index = findFirstNotOf(
            _currentLine,
            [&predicate](char c) { return c == '\'' || predicate(c); },
            _currentColumnIndex + offset);

        return (index != std::string_view::npos ? index : _currentLine.size()) - _currentColumnIndex;
    };

    // could be 0b.. or 0x..
    if (current() == '0')
    {
        switch (peek(1))
        {
        case 'b':
        case 'B':
            if (isBinaryDigit(peek(2)))
            {
                addToken(findFirstNotLiteral(isBinaryDigit, 3), Lexeme::BinaryLiteral);
            }
            else
            {
                _diagnosis.error(DiagnosisMessage::binaryLiteralInvalidFormat(), currentLocation());
                next();
            }
            return true;

        case 'x':
        case 'X':
            if (isHexadecimalDigit(peek(2)))
            {
                addToken(findFirstNotLiteral(isHexadecimalDigit, 3), Lexeme::HexadecimalLiteral);
            }
            else
            {
                _diagnosis.error(DiagnosisMessage::hexadecimalLiteralInvalidFormat(), currentLocation());
                next();
            }
            return true;
        }
    }
    else if (!isDigit(current()))
    {
        return false;
    }

    // could be decimal-literal or floating-literal
    std::size_t offset = findFirstNotLiteral(isDigit, 1);

    if (peek(offset) != '.')
    {
        addToken(offset, Lexeme::DecimalLiteral);
    }
    else
    {
        ++offset;

        if (!isDigit(peek(offset)))
        {
            _diagnosis.error(DiagnosisMessage::floatingLiteralInvalidFormat(_currentLine.substr(_currentColumnIndex, offset)), currentLocation());
        }
        else
        {
            addToken(findFirstNotLiteral(isDigit, offset), Lexeme::FloatingLiteral);
        }
    }

    return true;
}

// { encoding-prefix }? " { s-char }* "
bool Lexer::tryLexStringLiteral()
{
    static constexpr char Quote = '\"';

    std::size_t offset = findQuoteBeginOffset(Quote);

    if (offset == std::string_view::npos)
    {
        return false;
    }

    for (std::size_t index{0}; peek(offset) != Quote; offset = index - _currentColumnIndex + 1)
    {
        if (_currentColumnIndex + offset >= _currentLine.size())
        {
            break;
        }

        index = findCharacterEndIndex(offset, Quote);

        if (index == std::string_view::npos)
        {
            // can't find end of string literal
            offset = std::string_view::npos;
            break;
        }
    }

    // if valid, the previous character should never be a \ at this point
    assert(offset == std::string_view::npos || (offset > 0 && peek(offset - 1) != '\\'));

    std::size_t endIndex = offset == std::string_view::npos ? std::string_view::npos : _currentColumnIndex + offset;

    if (!validateQuotedLiteralEndIndex(endIndex, Quote))
    {
        endIndex = _currentLine.size();
    }

    addToken(endIndex - _currentColumnIndex + 1, Lexeme::StringLiteral);

    return true;
}

bool Lexer::tryLexKeyword()
{
    // clang-format off
    return tryLexKeywordContainer<BooleanLiteral>(BooleanLiteralStrings, isAlphabetic) ||
           tryLexKeywordContainer<FunctionModifier>(FunctionModifierStrings, isAlphabetic) ||
           tryLexKeywordContainer<Keyword>(KeywordStrings, [](char c){ return isAlphanumeric(c) || c == '_'; }) ||
           tryLexKeywordContainer<ParameterModifier>(ParameterModifierStrings, isAlphabetic) ||
           tryLexKeywordContainer<PointerLiteral>(PointerLiteralStrings, isAlphabetic);
    // clang-format on
}

bool Lexer::tryLexIdentifier()
{
    const std::size_t index = findFirstIdentifierEnd(_currentLine, _currentColumnIndex);

    if (index == std::string_view::npos)
    {
        return false;
    }

    const std::size_t offset = index - _currentColumnIndex;

    addToken(offset, Lexeme::Identifier);

    return true;
}

template<typename T>
bool Lexer::tryLexKeywordContainer(auto& container, auto isValidChar)
{
    std::size_t containerIndex{0};

    const std::size_t end = _currentLine.size() - _currentColumnIndex;

    std::size_t i = 0;

    auto findFirstWithChar = [&container, &containerIndex, &i](char current) {
        for (; containerIndex < container.size(); ++containerIndex)
        {
            const auto& text = container[containerIndex];

            if (i >= text.size())
            {
                continue;
            }

            if (current == text[i])
            {
                return containerIndex;
            }

            if (current < text[i])
            {
                return container.size();
            }
        }

        return container.size();
    };

    do
    {
        containerIndex = findFirstWithChar(peek(i));

        if (containerIndex >= container.size())
        {
            return false;
        }

        ++i;

        if (container[containerIndex].size() == i)
        {
            const char v = peek(i);

            if (v == '\0' || !isValidChar(v))
            {
                addToken(i, Lexeme(static_cast<T>(containerIndex)));
                return true;
            }
        }

    } while (i < end && isValidChar(peek(i)));

    return false;
}

// { encoding-prefix }?{ quote }
std::size_t Lexer::findQuoteBeginOffset(char quote) const
{
    const char currentChar = current();

    if (currentChar == quote)
    {
        return 1;
    }

    // encoding-prefix: { u | u8 }?
    if (currentChar == 'u')
    {
        const char nextChar = peek(1);

        if (nextChar == quote)
        {
            return 2;
        }

        if (nextChar == '8' && peek(2) == quote)
        {
            return 3;
        }
    }

    return std::string_view::npos;
}

std::size_t Lexer::findCharacterEndIndex(std::size_t offset, char quote)
{
    std::size_t endIndex = findUniversalCharacterEndIndex(offset);

    if (endIndex == std::string_view::npos)
    {
        endIndex = findEscapeSequenceEndIndex(offset);

        if (endIndex == std::string_view::npos)
        {
            endIndex = findBasicCharacterEndIndex(offset, quote);
        }
    }

    return endIndex;
}

// universal-character-name
//    \u { hexadecimal-digit }4
//    \U { hexadecimal-digit }8
std::size_t Lexer::findUniversalCharacterEndIndex(std::size_t startOffset)
{
    static constexpr std::size_t SmallUniversalCharacterNameLength{4};
    static constexpr std::size_t BigUniversalCharacterNameLength{8};

    std::size_t offset = startOffset;

    if (peek(offset) != '\\')
    {
        return std::string_view::npos;
    }

    const char u = peek(offset + 1);

    const std::size_t requiredSize = [u]() -> std::size_t {
        switch (u)
        {
        case 'u': return SmallUniversalCharacterNameLength;
        case 'U': return BigUniversalCharacterNameLength;
        default:
            return 0ULL;
        }
    }();

    // it's not a universal-character-name
    if (requiredSize == 0)
    {
        return std::string_view::npos;
    }

    offset += 2;

    const std::size_t start = offset;

    std::size_t end = start + requiredSize;

    if (end >= _currentLine.size())
    {
        // we'll report and error, but consume the character anyway
        end = _currentLine.size() - 1;
    }

    for (; offset <= end; ++offset)
    {
        if (!isHexadecimalDigit(peek(offset)))
        {
            break;
        }
    }

    if (offset - start != requiredSize)
    {
        _diagnosis.error(DiagnosisMessage::universalCharacterNameInvalidFormat(u, requiredSize),
                         SourceLocation{.line = static_cast<SourceLine>(_currentLineIndex),
                                        .column = static_cast<SourceColumn>(_currentColumnIndex + startOffset)});

        // do not return, consume the characters
    }

    return _currentColumnIndex + offset - startOffset;
}

std::size_t Lexer::findEscapeSequenceEndIndex(std::size_t offset) const
{
    if (peek(offset) != '\\')
    {
        return std::string_view::npos;
    }

    const char nextChar = peek(offset + 1);

    if (nextChar == 'x')
    {
        const std::size_t index = findFirstNotOf(_currentLine, isHexadecimalDigit, _currentColumnIndex + offset + 2);

        if (index != std::string_view::npos)
        {
            return index - 1;
        }
    }
    else if (nextChar != '\\')
    {
        assert(nextChar != 'u' && nextChar != 'U'); // validated in findUniversalCharacterEndIndex
        return _currentColumnIndex + offset + 1;
    }

    return std::string_view::npos;
}

std::size_t Lexer::findBasicCharacterEndIndex(std::size_t offset, char quote) const
{
    const char c = peek(offset);

    return c != quote && c != '\\' ? _currentColumnIndex + offset : std::string_view::npos;
}

bool Lexer::validateQuotedLiteralEndIndex(std::size_t endIndex, char quote)
{
    assert(quote == '\'' || quote == '"');

    const bool isEndInvalid = endIndex == std::string_view::npos || endIndex >= _currentLine.size();

    if (isEndInvalid || _currentLine[endIndex] != quote)
    {
        _diagnosis.error(
            quote == '\'' ? DiagnosisMessage::characterLiteralMissingClosingQuote() : DiagnosisMessage::stringLiteralMissingClosingQuote(),
            SourceLocation{
                .line = static_cast<SourceLine>(_currentLineIndex),
                .column = isEndInvalid ? InvalidSourceColumn : static_cast<SourceColumn>(endIndex)});

        return false;
    }

    return true;
}

void Lexer::next()
{
    ++_currentColumnIndex;
}

char Lexer::current() const
{
    return _currentLine[_currentColumnIndex];
}

char Lexer::peek(std::size_t offset) const
{
    const std::size_t i = _currentColumnIndex + offset;
    return i < _currentLine.size() ? _currentLine[i] : '\0';
}

SourceLocation Lexer::currentLocation() const
{
    return SourceLocation{.line = static_cast<SourceLine>(_currentLineIndex), .column = static_cast<SourceColumn>(_currentColumnIndex)};
}

void Lexer::addToken(std::size_t size, Lexeme lexeme)
{
    const std::string_view text = _currentLine.substr(_currentColumnIndex, size);

    _tokens.push_back(Token{
        .lexeme = lexeme,
        .location = currentLocation(),
        .text = text});

    _currentColumnIndex += size - 1;
}

void Lexer::addToken(std::size_t size, Lexeme::Basic lexeme)
{
    addToken(size, Lexeme{lexeme});
}

void Lexer::addTernaryConditionalToken(char conditionCharacter, Lexeme::Basic trueValue, Lexeme::Basic falseValue)
{
    if (peek(1) == conditionCharacter)
    {
        addToken(2, trueValue);
    }
    else
    {
        addToken(1, falseValue);
    }
}

void Lexer::addComment(std::string_view text, SourceLocation beginLocation, SourceLocation endLocation)
{
    _comments.push_back(
        {.text = std::string(text),
         .beginLocation = beginLocation,
         .endLocation = endLocation,
         .type = Comment::Type::Line});
}

void Lexer::beginComment()
{
    assert(!_isInComment);

    _isInComment = true;

    _comments.push_back(
        {.text = "/*",
         .beginLocation = currentLocation(),
         .endLocation = {},
         .type = Comment::Type::Block});
}

void Lexer::endComment()
{
    assert(_isInComment);

    assert(!_comments.empty());

    assert(_comments.back().type == Comment::Type::Block);
    assert(_comments.back().beginLocation != InvalidSourceLocation);
    assert(_comments.back().endLocation == InvalidSourceLocation);
    assert(!_comments.back().text.empty());

    _comments.back().endLocation = currentLocation();

    _isInComment = false;
}

} // namespace CPPS
