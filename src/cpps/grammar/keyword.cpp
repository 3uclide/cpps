#include "cpps/grammar/keyword.hpp"

#include <algorithm>

namespace CPPS {

static_assert(
    KeywordStrings == [] { std::array strings = KeywordStrings; std::ranges::sort(strings); return strings; }(),
    "Keyword must be sorted");

} // namespace CPPS