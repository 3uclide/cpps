#include "cpps/grammar/keyword.hpp"

#include <algorithm>

namespace CPPS {

static_assert(
    KeywordStrings == [] { std::array strings = KeywordStrings; std::ranges::sort(strings); return strings; }(),
    "Keyword must be sorted");

std::ostream& operator<<(std::ostream& os, Keyword value)
{
    return os << fmt::format("{}", value);
}

} // namespace CPPS