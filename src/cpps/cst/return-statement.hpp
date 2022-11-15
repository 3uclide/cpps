#pragma once

#include "cpps/cst/node.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token-ref.hpp"
#include "cpps/token.hpp"

namespace CPPS::CST {

struct Expression;

struct ReturnStatement
{
    ~ReturnStatement();

    [[nodiscard]] SourceLocation getLocation() const;

    TokenRef identifier;
    Node<Expression> expression;
};

inline SourceLocation ReturnStatement::getLocation() const
{
    return identifier.get().location;
}

} // namespace CPPS::CST
