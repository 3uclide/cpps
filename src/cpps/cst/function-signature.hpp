#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/identifier-expression.hpp"
#include "cpps/cst/parameter-declaration-list.hpp"

namespace CPPS::CST {

struct FunctionSignature
{
    using returns_t = std::variant<
        std::monostate,
        IdentifierExpression,
        ParameterDeclarationList>;

    ParameterDeclarationList parameters;
    returns_t returns;
    bool throws{false};
};

} // namespace CPPS::CST
