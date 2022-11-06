#pragma once

#include <array>
#include <string_view>

namespace CPPS {

enum class BooleanLiteral
{
    False,
    True
};

inline constexpr std::array BooleanLiteralStrings = std::to_array<std::string_view>(
    {"false",
     "true"});

inline constexpr std::size_t BooleanLiteralCount = BooleanLiteralStrings.size();

constexpr std::string_view toStringView(BooleanLiteral value)
{
    return BooleanLiteralStrings[static_cast<std::size_t>(value)];
}

} // namespace CPPS
