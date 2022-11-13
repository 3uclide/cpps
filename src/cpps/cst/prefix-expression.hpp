#pragma once

#include <vector>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/postfix-expression.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct PrefixExpression : PostfixExpression
{
    std::vector<TokenRef> ops;
};

} // namespace CPPS::CST
