#include "cpps/grammar/parameter-modifier.hpp"

#include <algorithm>

namespace CPPS {

static_assert(
    ParameterModifierStrings == [] { std::array strings = ParameterModifierStrings; std::ranges::sort(strings); return strings; }(),
    "ParameterModifier must be sorted");

} // namespace CPPS