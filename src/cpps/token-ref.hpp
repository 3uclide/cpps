#pragma once

#include <functional>

namespace CPPS {
class Token;
} // namespace CPPS

namespace CPPS::CST {

using TokenRef = std::reference_wrapper<const Token>;

} // namespace CPPS::CST
