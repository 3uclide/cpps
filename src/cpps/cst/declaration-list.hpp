#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/declaration.hpp"

namespace CPPS::CST {

class DeclarationList : public List<Declaration>
{
public:
    using List<Declaration>::List;
};

} // namespace CPPS::CST
