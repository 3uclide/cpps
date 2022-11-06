#pragma once

#include <fmt/core.h>
#include <optional>
#include <ostream>
#include <string>

namespace cpps {

using SourceLine = std::uint32_t;
using SourceColumn = std::uint32_t;

inline constexpr SourceLine InvalidSourceLine{std::numeric_limits<SourceLine>::max()};
inline constexpr SourceColumn InvalidSourceColumn{std::numeric_limits<SourceColumn>::max()};

struct SourceLocation
{
    bool operator==(const SourceLocation&) const = default;

    SourceLine line{InvalidSourceLine};
    SourceColumn column{InvalidSourceColumn};
};

inline constexpr SourceLocation InvalidSourceLocation{InvalidSourceLine, InvalidSourceColumn};

std::ostream& operator<<(std::ostream& os, const SourceLocation& value);
std::ostream& operator<<(std::ostream& os, const std::optional<SourceLocation>& value);

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

        return fmt::format_to(ctx.out(), "invalid");
    }
};

namespace cpps {

inline std::ostream& operator<<(std::ostream& os, const SourceLocation& value)
{
    return os << fmt::format("{}", value);
}

inline std::ostream& operator<<(std::ostream& os, const std::optional<SourceLocation>& value)
{
    if (value)
    {
        return os << fmt::format("{}", *value);
    }

    return os << "nullopt";
}

} // namespace cpps
