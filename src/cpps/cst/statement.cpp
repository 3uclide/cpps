#include "cpps/cst/statement.hpp"

#include "cpps/cst/.include-types.hpp"

namespace CPPS::CST {

SourceLocation Statement::getLocation() const
{
    if (isType<Declaration>()) { return getType<Declaration>().getLocation(); }
    if (isType<CompoundStatement>()) { return getType<CompoundStatement>().getLocation(); }
    if (isType<ExpressionStatement>()) { return getType<ExpressionStatement>().getLocation(); }
    if (isType<IterationStatement>()) { return getType<IterationStatement>().getLocation(); }
    if (isType<ReturnStatement>()) { return getType<ReturnStatement>().getLocation(); }
    if (isType<SelectionStatement>()) { return getType<SelectionStatement>().getLocation(); }

    return InvalidSourceLocation;
}

} // namespace CPPS::CST
