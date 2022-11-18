#pragma once

#include "cpps/cst/node.hpp"

namespace CPPS {

struct SourceLocation;

namespace CST {

struct AssignmentExpression;

struct Expression
{
    [[nodiscard]] SourceLocation getLocation() const;

    Node<AssignmentExpression> assignment;
};

} // namespace CST
} // namespace CPPS
