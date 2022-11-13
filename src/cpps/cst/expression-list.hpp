#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/list.hpp"

namespace CPPS::CST {

class ExpressionList : public List<Expression>
{
public:
    using List<Expression>::List;
};

} // namespace CPPS::CST
