#pragma once

#include <memory>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

struct ExpressionStatement
{
    [[nodiscard]] SourceLocation getLocation() const;

    std::unique_ptr<Expression> expression;
    bool hasSemicolon{false};
};

} // namespace CPPS::CST
