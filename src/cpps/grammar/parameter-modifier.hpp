#pragma once

#include <array>
#include <ostream>
#include <string_view>

#include <fmt/format.h>

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

std::ostream& operator<<(std::ostream& os, ParameterModifier value);

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

template<>
struct fmt::formatter<CPPS::ParameterModifier> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(CPPS::ParameterModifier value, FormatContext& ctx) const
    {
        return formatter<std::string_view>::format(toStringView(value), ctx);
    }
};
