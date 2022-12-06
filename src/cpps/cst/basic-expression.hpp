#pragma once

#include "cpps/cst/node.hpp"

namespace CPPS {

struct SourceLocation;

namespace CST {

struct PrefixExpression;
struct PostfixExpression;
struct PrimaryExpression;

struct BasicExpression
{
    [[nodiscard]] SourceLocation getLocation() const;

    Node<PrefixExpression> prefix;
    Node<PrimaryExpression> primary;
    Node<PostfixExpression> postfix;
};

} // namespace CST
} // namespace CPPS
