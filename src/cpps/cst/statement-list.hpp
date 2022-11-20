#pragma once

#include "cpps/cst/node-list.hpp"

namespace CPPS::CST {

struct Statement;

struct StatementList : NodeList<Statement>
{
    using NodeList<Statement>::NodeList;

    StatementList() = default;
    StatementList(StatementList&& other) = default;
    StatementList& operator=(StatementList&& other) = default;

    ~StatementList();
};

} // namespace CPPS::CST
