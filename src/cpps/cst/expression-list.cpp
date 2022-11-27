#include "cpps/cst/expression-list.hpp"

#include "cpps/cst.hpp"

namespace CPPS::CST {

ExpressionTerm::~ExpressionTerm() = default;

ExpressionTerm::ExpressionTerm(Node<Expression>&& expr, ParameterModifier mod)
    : expression(std::move(expr))
    , modifier(mod)
{
}

SourceLocation ExpressionTerm::getLocation() const
{
    return expression->getLocation();
}

ExpressionList::~ExpressionList() = default;

} // namespace CPPS::CST
