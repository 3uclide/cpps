#pragma once

#include "cpps/cst/declaration-list.hpp"
#include "cpps/utility/bump-pointer-allocator.hpp"

namespace CPPS::CST {

struct TranslationUnit
{
    using Allocator = BumpPointerAllocator<1024ULL * 500ULL>;

    TranslationUnit() = default;
    TranslationUnit(TranslationUnit&&) = default;
    TranslationUnit& operator=(TranslationUnit&&) = default;

    DeclarationList declarations;

    Allocator allocator;
};

} // namespace CPPS::CST
