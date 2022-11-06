#pragma once

#include <array>
#include <string_view>

namespace CPPS {

enum class FunctionModifier
{
    Final,
    Implicit,
    Override,
    Virtual
};

inline constexpr std::array FunctionModifierStrings = std::to_array<std::string_view>(
    {"final",
     "implicit",
     "override",
     "virtual"});

inline constexpr std::size_t FunctionModifierCount = FunctionModifierStrings.size();

constexpr std::string_view toStringView(FunctionModifier value)
{
    return FunctionModifierStrings[static_cast<std::size_t>(value)];
}

} // namespace CPPS
