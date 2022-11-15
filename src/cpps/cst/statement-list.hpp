#pragma once

#include "cpps/cst/node-list.hpp"

namespace CPPS::CST {

struct Statement;

struct StatementList : NodeList<Statement>
{
    using NodeList<Statement>::NodeList;

    ~StatementList();
};

} // namespace CPPS::CST
