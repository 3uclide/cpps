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

struct PrimaryExpression
{
    using Type = NodeVariant<std::monostate, IdentifierExpression, Declaration, Token, ExpressionList>;

    ~PrimaryExpression();

    [[nodiscard]] SourceLocation getLocation() const;

    Type type;
};

} // namespace CPPS::CST
