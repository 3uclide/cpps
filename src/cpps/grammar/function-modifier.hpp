#pragma once

#include <array>
#include <fmt/format.h>
#include <ostream>
#include <string_view>

namespace CPPS {

enum class FunctionModifier
{
    Final,
    Implicit,
    Override,
    Virtual
};

std::ostream& operator<<(std::ostream& os, FunctionModifier value);

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

template<>
struct fmt::formatter<CPPS::FunctionModifier> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(CPPS::FunctionModifier value, FormatContext& ctx) const
    {
        return formatter<std::string_view>::format(toStringView(value), ctx);
    }
};
