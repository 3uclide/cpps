#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/declaration.hpp"

namespace CPPS::CST {

struct DeclarationList : List<Declaration>
{
    using List<Declaration>::List;
};

} // namespace CPPS::CST
