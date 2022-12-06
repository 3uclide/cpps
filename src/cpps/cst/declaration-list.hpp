#pragma once

#include "cpps/cst/node-list.hpp"

namespace CPPS::CST {

struct Declaration;

struct DeclarationList : NodeList<Declaration>
{
    using NodeList<Declaration>::NodeList;

    DeclarationList() = default;
    DeclarationList(DeclarationList&& other) = default;
    DeclarationList& operator=(DeclarationList&& other) = default;

    ~DeclarationList();
};

} // namespace CPPS::CST
