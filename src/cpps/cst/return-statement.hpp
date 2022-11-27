#pragma once

#include "cpps/cst/node.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token-ref.hpp"
#include "cpps/token.hpp"

namespace CPPS::CST {

struct Expression;

struct ReturnStatement
{
    explicit ReturnStatement(const Token& id);

    ~ReturnStatement();

    [[nodiscard]] SourceLocation getLocation() const;

    TokenRef identifier;
    Node<Expression> expression;
};

inline ReturnStatement::ReturnStatement(const Token& id)
    : identifier(id)
{
}

inline SourceLocation ReturnStatement::getLocation() const
{
    return identifier.get().location;
}

} // namespace CPPS::CST
