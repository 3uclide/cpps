#pragma once

#include <string>
#include <string_view>

#include "cpps/lexeme.hpp"
#include "cpps/source-location.hpp"

namespace CPPS {

class Token
{
public:
    static const Token& invalid();

public:
    constexpr Token() = default;
    constexpr Token(Lexeme lexeme_, SourceLocation src, std::string_view text_);

    bool operator==(const Token&) const = default;

    Lexeme lexeme;
    SourceLocation location;
    std::string_view text;
};

std::ostream& operator<<(std::ostream& os, const Token& value);

inline const Token& Token::invalid()
{
    static const Token Invalid{};
    return Invalid;
}

constexpr Token::Token(Lexeme lexeme_, SourceLocation src, std::string_view text_)
    : lexeme(lexeme_)
    , location(src)
    , text(text_)
{
}

} // namespace CPPS

template<>
struct fmt::formatter<CPPS::Token> : formatter<std::string>
{
    template<typename FormatContext>
    auto format(CPPS::Token token, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "[{}] {}: '{}'", token.location, token.lexeme, token.text);
    }
};

namespace CPPS {

inline std::ostream& operator<<(std::ostream& os, const Token& value)
{
    return os << fmt::format("{}", value);
}

} // namespace CPPS
