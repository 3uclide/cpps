#pragma once

#include "cpps/cst/node.hpp"
#include "cpps/cst/parameter-modifier.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

struct Declaration;

struct ParameterDeclaration
{
    ~ParameterDeclaration();

    Node<Declaration> declaration;

    ParameterModifier modifier{ParameterModifier::In};

    SourceLocation location;
};

} // namespace CPPS::CST
