#pragma once

#include <memory>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/declaration.hpp"
#include "cpps/cst/parameter-modifier.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

class ParameterDeclaration
{
public:
    std::unique_ptr<Declaration> _declaration;

    ParameterModifier _modifier{ParameterModifier::In};

    SourceLocation location;
};

} // namespace CPPS::CST
