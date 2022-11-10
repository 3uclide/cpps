#pragma once

#include <string>
#include <string_view>

#include "cpps/lexeme.hpp"
#include "cpps/source-location.hpp"

namespace CPPS {

struct Token
{
    bool operator==(const Token&) const = default;

    Lexeme lexeme;
    SourceLocation location;
    std::string_view text;
};

std::ostream& operator<<(std::ostream& os, const Token& value);

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
