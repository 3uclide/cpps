#pragma once

#include <array>
#include <ostream>
#include <string_view>

#include <fmt/format.h>

namespace CPPS {

enum class Punctuator
{
    Ampersand,
    AmpersandEqual,
    Arrow,
    Assignment,
    Caret,
    CaretEqual,
    CloseBrace,
    CloseBracket,
    CloseParenthesis,
    Colon,
    Comma,
    CompareEqual,
    CompareNotEqual,
    Dollar,
    Dot,
    DoubleColon,
    Ellipsis,
    Greater,
    GreaterEqual,
    LeftShift,
    LeftShiftEqual,
    Less,
    LessEqual,
    LogicalAnd,
    LogicalAndEqual,
    LogicalOr,
    LogicalOrEqual,
    Minus,
    MinusEqual,
    MinusMinus,
    Modulo,
    ModuloEqual,
    Not,
    Multiply,
    MultiplyEqual,
    OpenBrace,
    OpenBracket,
    OpenParenthesis,
    Pipe,
    PipeEqual,
    Plus,
    PlusEqual,
    PlusPlus,
    QuestionMark,
    RightShift,
    RightShiftEqual,
    Semicolon,
    Slash,
    SlashEqual,
    Spaceship,
    Tilde,
    TildeEqual
};

std::ostream& operator<<(std::ostream& os, Punctuator value);

static constexpr std::array PunctuatorStrings = std::to_array<std::string_view>(
    {"&",
     "&=",
     "->",
     "=",
     "^",
     "^=",
     "}",
     "]",
     ")",
     ":",
     ",",
     "==",
     "!=",
     "$",
     ".",
     "::",
     "...",
     ">",
     ">=",
     "<<",
     "<<=",
     "<",
     "<=",
     "&&",
     "&&=",
     "||",
     "||=",
     "-",
     "-=",
     "--",
     "%",
     "%=",
     "!",
     "*",
     "*=",
     "{",
     "[",
     "(",
     "|",
     "|=",
     "+",
     "+=",
     "++",
     "?",
     ">>",
     ">>=",
     ";",
     "/",
     "/=",
     "<=>",
     "~",
     "~="});

inline constexpr std::size_t PunctuatorCount = PunctuatorStrings.size();

constexpr std::string_view toStringView(Punctuator value)
{
    return PunctuatorStrings[static_cast<std::size_t>(value)];
}

} // namespace CPPS

template<>
struct fmt::formatter<CPPS::Punctuator> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(CPPS::Punctuator value, FormatContext& ctx) const
    {
        return formatter<std::string_view>::format(toStringView(value), ctx);
    }
};
