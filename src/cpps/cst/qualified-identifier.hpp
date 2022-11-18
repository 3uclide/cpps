#pragma once

#include <vector>

#include "cpps/cst/unqualified-identifier.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct QualifiedIdentifier
{
    struct Term
    {
        TokenRef scope;
        UnqualifiedIdentifier identifier;
    };

    std::vector<Term> terms;
};

} // namespace CPPS::CST
