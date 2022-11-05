#pragma once

#include <fmt/core.h>
#include <string>

namespace cpps {

using SourceLine = std::uint32_t;
using SourceColumn = std::uint32_t;

inline constexpr SourceLine InvalidSourceLine{std::numeric_limits<SourceLine>::max()};
inline constexpr SourceColumn InvalidSourceColumn{std::numeric_limits<SourceColumn>::max()};

struct SourceLocation
{
    bool operator==(const SourceLocation&) const = default;

    SourceLine line;
    SourceColumn column;
};

inline constexpr SourceLocation InvalidSourceLocation{InvalidSourceLine, InvalidSourceColumn};

} // namespace cpps

template<>
struct fmt::formatter<cpps::SourceLocation> : formatter<std::string>
{
    template<typename FormatContext>
    auto format(cpps::SourceLocation location, FormatContext& ctx) const
    {
        if (location.line != cpps::InvalidSourceLine)
        {
            if (location.column != cpps::InvalidSourceColumn)
            {
                return fmt::format_to(ctx.out(), "{}, {}", location.line, location.column);
            }

            return fmt::format_to(ctx.out(), "{}", location.line);
        }

        return ctx.out();
    }
};
