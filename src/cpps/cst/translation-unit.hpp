#pragma once

#include "cpps/cst/declaration.hpp"
#include "cpps/cst/declaration-list.hpp"

namespace CPPS::CST {

struct TranslationUnit
{
    DeclarationList declarations;
};

} // namespace CPPS::CST
