#pragma once

#include <vector>

#include "cpps/cst/unqualified-identifier.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct QualifiedIdentifier
{
    struct term
    {
        TokenRef scope;
        UnqualifiedIdentifier identifier;
    };

    std::vector<term> terms;
};

} // namespace CPPS::CST
