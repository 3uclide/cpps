#pragma once

#include "cpps/cst/node.hpp"

namespace CPPS {
struct SourceLocation;
}

namespace CPPS::CST {

struct Expression;

struct ExpressionStatement
{
    ~ExpressionStatement();

    [[nodiscard]] SourceLocation getLocation() const;

    Node<Expression> expression;
    bool hasSemicolon{false};
};

} // namespace CPPS::CST
