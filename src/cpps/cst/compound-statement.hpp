#pragma once

#include "cpps/cst/statement-list.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

class CompoundStatement : public StatementList
{
public:
    SourceLocation getLocation() const;
    SourceLocation getOpenBraceLocation() const;
    SourceLocation getCloseBraceLocation() const;

private:
    SourceLocation _openBraceLocation;
    SourceLocation _closeBraceLocation;
};

inline SourceLocation CompoundStatement::getLocation() const
{
    return getOpenBraceLocation();
}

inline SourceLocation CompoundStatement::getOpenBraceLocation() const
{
    return _openBraceLocation;
}

inline SourceLocation CompoundStatement::getCloseBraceLocation() const
{
    return _closeBraceLocation;
}

} // namespace CPPS::CST
