#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/list.hpp"

namespace CPPS::CST {

struct ParameterDeclarationList : List<ParameterDeclaration>
{
    using List<ParameterDeclaration>::List;
};

} // namespace CPPS::CST
