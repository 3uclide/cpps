#pragma once

#include "cpps/cst/statement-list.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

struct CompoundStatement : StatementList
{
    SourceLocation getLocation() const;

    SourceLocation openBraceLocation;
    SourceLocation closeBraceLocation;
};

inline SourceLocation CompoundStatement::getLocation() const
{
    return openBraceLocation;
}

} // namespace CPPS::CST
