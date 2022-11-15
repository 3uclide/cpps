#include "cpps/cst/statement.hpp"

#include "cpps/cst.hpp"

namespace CPPS::CST {

Statement::~Statement() = default;

SourceLocation Statement::getLocation() const
{
    if (type.is<Declaration>()) { return type.as<Declaration>().getLocation(); }
    if (type.is<CompoundStatement>()) { return type.as<CompoundStatement>().getLocation(); }
    if (type.is<ExpressionStatement>()) { return type.as<ExpressionStatement>().getLocation(); }
    if (type.is<IterationStatement>()) { return type.as<IterationStatement>().getLocation(); }
    if (type.is<ReturnStatement>()) { return type.as<ReturnStatement>().getLocation(); }
    if (type.is<SelectionStatement>()) { return type.as<SelectionStatement>().getLocation(); }

    return InvalidSourceLocation;
}

} // namespace CPPS::CST
