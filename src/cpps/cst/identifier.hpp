#pragma once

#include "cpps/cst/node-variant.hpp"

namespace CPPS::CST {

struct QualifiedIdentifier;
struct UnqualifiedIdentifier;

struct Identifier
{
    using Type = NodeVariant<QualifiedIdentifier, UnqualifiedIdentifier>;

    ~Identifier();

    Type type;
};

} // namespace CPPS::CST
