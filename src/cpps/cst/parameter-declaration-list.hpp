#pragma once

#include "cpps/cst/node-list.hpp"

namespace CPPS::CST {

struct ParameterDeclaration;

struct ParameterDeclarationList : NodeList<ParameterDeclaration>
{
    using NodeList<ParameterDeclaration>::NodeList;

    ~ParameterDeclarationList();
};

} // namespace CPPS::CST
