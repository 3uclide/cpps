#pragma once

#include "cpps/cst/node-variant.hpp"

namespace CPPS {
class Token;
struct SourceLocation;
} // namespace CPPS

namespace CPPS::CST {

struct Declaration;
struct ExpressionList;
struct IdentifierExpression;

using BasePrimaryExpression = NodeVariant<std::monostate, IdentifierExpression, Declaration, Token, ExpressionList>;

struct PrimaryExpression : BasePrimaryExpression
{
    using BasePrimaryExpression::BasePrimaryExpression;

    ~PrimaryExpression();

    [[nodiscard]] SourceLocation getLocation() const;
};

} // namespace CPPS::CST
