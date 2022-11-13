#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/identifier-expression.hpp"
#include "cpps/cst/parameter-declaration-list.hpp"

namespace CPPS::CST {

class FunctionSignature
{
    using returns_t = std::variant<
        std::monostate,
        IdentifierExpression,
        ParameterDeclarationList>;

    ParameterDeclarationList _parameters;
    returns_t _returns;
    bool _throws{false};
};

} // namespace CPPS::CST
