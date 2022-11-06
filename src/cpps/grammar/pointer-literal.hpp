#pragma once

#include <array>
#include <string_view>

namespace CPPS {

enum class PointerLiteral
{
    NullPtr
};

inline constexpr std::array PointerLiteralStrings = std::to_array<std::string_view>(
    {"nullptr"});

inline constexpr std::size_t PointerLiteralCount = PointerLiteralStrings.size();

constexpr std::string_view toStringView(PointerLiteral value)
{
    return PointerLiteralStrings[static_cast<std::size_t>(value)];
}

} // namespace CPPS
