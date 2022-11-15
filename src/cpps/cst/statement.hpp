#pragma once

#include "cpps/cst/node-variant.hpp"
#include "cpps/source-location.hpp"

namespace CPPS::CST {

struct Declaration;
struct CompoundStatement;
struct ExpressionStatement;
struct IterationStatement;
struct ReturnStatement;
struct SelectionStatement;

struct Statement
{
    ~Statement();

    using Type = NodeVariant<
        Declaration,
        CompoundStatement,
        ExpressionStatement,
        IterationStatement,
        ReturnStatement,
        SelectionStatement>;

    [[nodiscard]] SourceLocation getLocation() const;

    Type type;
};

} // namespace CPPS::CST
