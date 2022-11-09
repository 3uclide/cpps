#include "cpps/grammar/pointer-literal.hpp"

#include <algorithm>

namespace CPPS {

static_assert(
    PointerLiteralStrings == [] { std::array strings = PointerLiteralStrings; std::ranges::sort(strings); return strings; }(),
    "PointerLiteral must be sorted");

std::ostream& operator<<(std::ostream& os, PointerLiteral value)
{
    return os << fmt::format("{}", value);
}

} // namespace CPPS