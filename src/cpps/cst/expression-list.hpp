#pragma once

#include "cpps/cst/node-list.hpp"

namespace CPPS::CST {

struct Expression;

struct ExpressionList : NodeList<Expression>
{
    using NodeList<Expression>::NodeList;

    ~ExpressionList();
};

} // namespace CPPS::CST
