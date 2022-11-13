#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/list.hpp"

namespace CPPS::CST {

struct ExpressionList : List<Expression>
{
    using List<Expression>::List;
};

} // namespace CPPS::CST
