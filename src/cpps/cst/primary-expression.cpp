#include "cpps/cst/primary-expression.hpp"

#include <cassert>

#include "cpps/cst.hpp"

namespace CPPS::CST {

PrimaryExpression::~PrimaryExpression() = default;

SourceLocation PrimaryExpression::getLocation() const
{
    //if (type.is<Token>()) { return type.as<Token>().location; }
    if (type.is<ExpressionList>()) { return type.as<ExpressionList>().getLocation(); }
    if (type.is<IdentifierExpression>()) { return type.as<IdentifierExpression>().getLocation(); }
    if (type.is<Declaration>()) { return type.as<Declaration>().getLocation(); }

    assert(type.is<std::monostate>());

    return {0, 0};
}

} // namespace CPPS::CST
