#pragma once

#include <array>
#include <fmt/format.h>
#include <ostream>
#include <string_view>

namespace CPPS {

enum class PointerLiteral
{
    NullPtr
};

std::ostream& operator<<(std::ostream& os, PointerLiteral value);

inline constexpr std::array PointerLiteralStrings = std::to_array<std::string_view>(
    {"nullptr"});

inline constexpr std::size_t PointerLiteralCount = PointerLiteralStrings.size();

constexpr std::string_view toStringView(PointerLiteral value)
{
    return PointerLiteralStrings[static_cast<std::size_t>(value)];
}

} // namespace CPPS

template<>
struct fmt::formatter<CPPS::PointerLiteral> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(CPPS::PointerLiteral value, FormatContext& ctx) const
    {
        return formatter<std::string_view>::format(toStringView(value), ctx);
    }
};
