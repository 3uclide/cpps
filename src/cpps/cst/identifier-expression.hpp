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

struct IdentifierExpression
{
    SourceLocation getLocation() const;

    template<typename T>
    bool isType() const;

    template<typename T>
    const T& getType() const;

    Identifier type;
    SourceLocation location;
};

inline SourceLocation IdentifierExpression::getLocation() const
{
    return location;
}

template<typename T>
bool IdentifierExpression::isType() const
{
    return std::holds_alternative<T>(type);
}

template<typename T>
const T& IdentifierExpression::getType() const
{
    return std::get<T>(type);
}


} // namespace CPPS::CST
