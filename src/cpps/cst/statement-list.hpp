#pragma once

#include "cpps/cst/node-list.hpp"

namespace CPPS::CST {

struct Statement;

struct StatementList : NodeList<Statement>
{
    using NodeList<Statement>::NodeList;

    StatementList() = default;
    StatementList(StatementList&&) = default;
    StatementList& operator=(StatementList&&) = default;

    StatementList(const StatementList&) = delete;
    StatementList& operator=(const StatementList&) = delete;

    ~StatementList();
};

} // namespace CPPS::CST
