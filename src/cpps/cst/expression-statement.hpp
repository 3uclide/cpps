#pragma once

#include <memory>

#include "cpps/cst/.forward-declare-types.hpp"

namespace CPPS::CST {

struct ExpressionStatement
{
    std::unique_ptr<Expression> expression;
    bool hasSemicolon{false};
};

} // namespace CPPS::CST
