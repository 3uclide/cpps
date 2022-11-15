#pragma once

#include <vector>

#include "cpps/cst/postfix-expression.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token-ref.hpp"
#include "cpps/token.hpp"

namespace CPPS::CST {

struct PrefixExpression : PostfixExpression
{
    [[nodiscard]] SourceLocation getLocation() const;

    std::vector<TokenRef> ops;
};

inline SourceLocation PrefixExpression::getLocation() const
{
    return ops.empty() ? PostfixExpression::getLocation() : ops[0].get().location;
}

} // namespace CPPS::CST
