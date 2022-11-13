#pragma once

#include <memory>

#include "cpps/cst/.forward-declare-types.hpp"

namespace CPPS::CST {

class ExpressionStatement
{
public:
    std::unique_ptr<Expression> _expression;
    bool _hasSemicolon{false};
};

} // namespace CPPS::CST
