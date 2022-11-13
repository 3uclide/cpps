#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/list.hpp"

namespace CPPS::CST {

class StatementList : public List<Statement>
{
public:
    using List<Statement>::List;
};

} // namespace CPPS::CST
