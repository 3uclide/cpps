#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/token.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct ReturnStatement
{
    [[nodiscard]] SourceLocation getLocation() const;

    TokenRef identifier;
    std::unique_ptr<Expression> expression;

};

inline SourceLocation ReturnStatement::getLocation() const
{
    return identifier.get().location;
}

} // namespace CPPS::CST
