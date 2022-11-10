#include "cpps/grammar/punctuator.hpp"

namespace CPPS {

std::ostream& operator<<(std::ostream& os, Punctuator value)
{
    return os << fmt::format("{}", value);
}

} // namespace CPPS
