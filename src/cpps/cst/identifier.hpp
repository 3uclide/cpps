#pragma once

#include "cpps/cst/qualified-identifier.hpp"
#include "cpps/cst/unqualified-identifier.hpp"

namespace CPPS::CST {

using Identifier = std::variant<QualifiedIdentifier, UnqualifiedIdentifier>;

} // namespace CPPS::CST
