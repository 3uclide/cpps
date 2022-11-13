#pragma once

#include <memory>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/declaration.hpp"
#include "cpps/cst/parameter-modifier.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

struct ParameterDeclaration
{
    std::unique_ptr<Declaration> declaration;

    ParameterModifier modifier{ParameterModifier::In};

    SourceLocation location;
};

} // namespace CPPS::CST
