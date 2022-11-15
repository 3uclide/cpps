#pragma once

#include "cpps/cst/node-list.hpp"

namespace CPPS::CST {

struct Declaration;

struct DeclarationList : NodeList<Declaration>
{
    using NodeList<Declaration>::NodeList;

    ~DeclarationList();
};

} // namespace CPPS::CST
