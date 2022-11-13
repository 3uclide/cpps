#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/list.hpp"

namespace CPPS::CST {

struct StatementList : List<Statement>
{
    using List<Statement>::List;
};

} // namespace CPPS::CST
