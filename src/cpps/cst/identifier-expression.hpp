#pragma once

#include "cpps/cst/identifier.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

struct IdentifierExpression
{
    [[nodiscard]] SourceLocation getLocation() const;

    ~IdentifierExpression();

    Identifier identifier;
    SourceLocation location;
};

inline SourceLocation IdentifierExpression::getLocation() const
{
    return location;
}

} // namespace CPPS::CST
