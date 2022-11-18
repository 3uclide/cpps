#include "cpps/cst/primary-expression.hpp"

#include <cassert>

#include "cpps/cst.hpp"

namespace CPPS::CST {

PrimaryExpression::~PrimaryExpression() = default;

SourceLocation PrimaryExpression::getLocation() const
{
    //if (is<Token>()) { return as<Token>().location; }
    if (is<ExpressionList>()) { return as<ExpressionList>().getLocation(); }
    if (is<IdentifierExpression>()) { return as<IdentifierExpression>().getLocation(); }
    if (is<Declaration>()) { return as<Declaration>().getLocation(); }

    assert(is<std::monostate>());

    return {0, 0};
}

} // namespace CPPS::CST
