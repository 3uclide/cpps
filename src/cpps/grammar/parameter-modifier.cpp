#include "cpps/grammar/parameter-modifier.hpp"

#include <algorithm>

namespace CPPS {

static_assert(
    ParameterModifierStrings == [] { std::array strings = ParameterModifierStrings; std::ranges::sort(strings); return strings; }(),
    "ParameterModifier must be sorted");

std::ostream& operator<<(std::ostream& os, ParameterModifier value)
{
    return os << fmt::format("{}", value);
}

} // namespace CPPS