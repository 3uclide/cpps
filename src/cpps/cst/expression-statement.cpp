#include "cpps/cst/expression-statement.hpp"

#include "cpps/cst.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

ExpressionStatement::~ExpressionStatement() = default;

SourceLocation ExpressionStatement::getLocation() const
{
    return expression->getLocation();
}

} // namespace CPPS::CST
