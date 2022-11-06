#pragma once

#include <array>
#include <string_view>

namespace CPPS {

enum class ParameterModifier
{
    Copy,
    Forward,
    In,
    InOut,
    Move,
    Out
};

inline constexpr std::array ParameterModifierStrings = std::to_array<std::string_view>(
    {"copy",
     "forward",
     "in",
     "inout",
     "move",
     "out"});

inline constexpr std::size_t ParameterModifierCount = ParameterModifierStrings.size();

constexpr std::string_view toStringView(ParameterModifier value)
{
    return ParameterModifierStrings[static_cast<std::size_t>(value)];
}

} // namespace CPPS
