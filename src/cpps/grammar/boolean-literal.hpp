#pragma once

#include <array>
#include <ostream>
#include <string_view>

#include <fmt/format.h>

namespace CPPS {

enum class BooleanLiteral
{
    False,
    True
};

std::ostream& operator<<(std::ostream& os, BooleanLiteral value);

inline constexpr std::array BooleanLiteralStrings = std::to_array<std::string_view>(
    {"false",
     "true"});

inline constexpr std::size_t BooleanLiteralCount = BooleanLiteralStrings.size();

constexpr std::string_view toStringView(BooleanLiteral value)
{
    return BooleanLiteralStrings[static_cast<std::size_t>(value)];
}

} // namespace CPPS

template<>
struct fmt::formatter<CPPS::BooleanLiteral> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(CPPS::BooleanLiteral value, FormatContext& ctx) const
    {
        return formatter<std::string_view>::format(toStringView(value), ctx);
    }
};
