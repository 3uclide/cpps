#pragma once

#include "cpps/cst/node-variant.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS {
struct SourceLocation;
} // namespace CPPS

namespace CPPS::CST {

struct Declaration;
struct ExpressionList;
struct IdentifierExpression;

struct PrimaryExpression
{
    ~PrimaryExpression();

    using Type = NodeVariant<
        std::monostate,
        IdentifierExpression,
        Declaration,
        TokenRef,
        ExpressionList>;

    [[nodiscard]] SourceLocation getLocation() const;

    Type type;
};

} // namespace CPPS::CST
