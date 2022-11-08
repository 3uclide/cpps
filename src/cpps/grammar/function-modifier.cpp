#include "cpps/grammar/function-modifier.hpp"

#include <algorithm>

namespace CPPS {

static_assert(
    FunctionModifierStrings == [] { std::array strings = FunctionModifierStrings; std::ranges::sort(strings); return strings; }(),
    "FunctionModifier must be sorted");

std::ostream& operator<<(std::ostream& os, FunctionModifier value)
{
    return os << fmt::format("{}", value);
}

} // namespace CPPS
