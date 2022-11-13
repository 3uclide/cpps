#pragma once

#include <functional>
#include <variant>
#include <vector>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/qualified-identifier.hpp"
#include "cpps/cst/unqualified-identifier.hpp"
#include "cpps/cst/identifier.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

class IdentifierExpression
{
public:
    IdentifierExpression(QualifiedIdentifier&& qualifiedIdentifier, SourceLocation location);
    IdentifierExpression(UnqualifiedIdentifier&& unqualifiedIdentifier, SourceLocation location);

    template<typename T>
    bool is() const;

    template<typename T>
    const T& as() const;

    SourceLocation getLocation() const;

private:
    Identifier _identifier;
    SourceLocation _location;
};

} // namespace CPPS::CST
