#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/identifier-expression.hpp"
#include "cpps/cst/parameter-declaration-list.hpp"

namespace CPPS::CST {

struct FunctionSignature
{
    using Returns = std::variant<
        std::monostate,
        IdentifierExpression,
        ParameterDeclarationList>;

    ParameterDeclarationList parameters;
    Returns returns;
    bool throws{false};
};

} // namespace CPPS::CST
