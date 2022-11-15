#pragma once

#include "cpps/cst/parameter-declaration-list.hpp"
#include "cpps/cst/node-variant.hpp"

namespace CPPS::CST {

struct IdentifierExpression;

struct FunctionSignature
{
    using Returns = NodeVariant<
        std::monostate,
        IdentifierExpression,
        ParameterDeclarationList>;

    ~FunctionSignature();

    ParameterDeclarationList parameters;
    Returns returns;
    bool throws{false};
};

} // namespace CPPS::CST
