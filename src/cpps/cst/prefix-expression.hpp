#pragma once

#include <vector>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/postfix-expression.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct PrefixExpression : PostfixExpression
{
    SourceLocation getLocation() const;

    std::vector<TokenRef> ops;
};

inline SourceLocation PrefixExpression::getLocation() const
{
    return ops.empty() ? PostfixExpression::getLocation() : ops[0].get().location;
}

} // namespace CPPS::CST
