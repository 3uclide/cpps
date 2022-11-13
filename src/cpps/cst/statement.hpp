#pragma once

#include <variant>
#include <memory>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/compound-statement.hpp"
#include "cpps/cst/declaration.hpp"
#include "cpps/cst/expression-statement.hpp"
#include "cpps/cst/iteration-statement.hpp"
#include "cpps/cst/return-statement.hpp"
#include "cpps/cst/selection-statement.hpp"

namespace CPPS::CST {

struct Statement
{
    using Type = std::variant<
        std::unique_ptr<Declaration>,
        CompoundStatement,
        ExpressionStatement,
        IterationStatement,
        ReturnStatement,
        SelectionStatement>;

    Type _type;
};

} // namespace CPPS::CST
