#pragma once

#include "cpps/cst/declaration-list.hpp"
#include "cpps/utility/bump-pointer-allocator.hpp"

namespace CPPS::CST {

struct TranslationUnit
{
    using Allocator = BumpPointerAllocator<1024ULL * 50ULL>;

    TranslationUnit() = default;
    TranslationUnit(TranslationUnit&&) = default;
    TranslationUnit& operator=(TranslationUnit&&) = default;

    // Must stay first to be destroyed last
    Allocator allocator;

    DeclarationList declarations;
};

} // namespace CPPS::CST
