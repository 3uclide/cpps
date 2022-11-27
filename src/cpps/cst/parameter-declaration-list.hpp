#pragma once

#include "cpps/cst/node-list.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

struct ParameterDeclaration;

struct ParameterDeclarationList : NodeList<ParameterDeclaration>
{
    using NodeList<ParameterDeclaration>::NodeList;

    ParameterDeclarationList() = default;
    ParameterDeclarationList(ParameterDeclarationList&& other) = default;
    ParameterDeclarationList& operator=(ParameterDeclarationList&& other) = default;

    ~ParameterDeclarationList();

    SourceLocation openParenthesisLocation;
    SourceLocation closeParenthesisLocation;
};

} // namespace CPPS::CST
