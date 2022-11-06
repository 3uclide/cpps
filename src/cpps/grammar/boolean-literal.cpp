#include "cpps/grammar/boolean-literal.hpp"

#include <algorithm>

namespace CPPS {

static_assert(
    BooleanLiteralStrings == [] { std::array strings = BooleanLiteralStrings; std::ranges::sort(strings); return strings; }(),
    "BooleanLiteral must be sorted");

std::ostream& operator<<(std::ostream& os, BooleanLiteral value)
{
    return os << fmt::format("{}", value);
}

} // namespace CPPS
