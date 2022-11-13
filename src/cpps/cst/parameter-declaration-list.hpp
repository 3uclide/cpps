#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/list.hpp"

namespace CPPS::CST {

class ParameterDeclarationList : public List<ParameterDeclaration>
{
public:
    using List<ParameterDeclaration>::List;
};

} // namespace CPPS::CST
