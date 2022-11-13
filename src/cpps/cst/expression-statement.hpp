#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/expression.hpp"

namespace CPPS::CST {

class ExpressionStatement
{
public:
    std::unique_ptr<Expression> _expression;
    bool _hasSemicolon{false};
};

} // namespace CPPS::CST
