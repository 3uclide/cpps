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

    template<typename T>
    [[nodiscard]] bool isType() const;

    template<typename T>
    const T& getType() const;

    [[nodiscard]] SourceLocation getLocation() const;

    Type type;
};

template<>
inline bool Statement::isType<Declaration>() const
{
    return std::holds_alternative<std::unique_ptr<Declaration>>(type);
}

template<typename T>
bool Statement::isType() const
{
    return std::holds_alternative<T>(type);
}

template<>
inline const Declaration& Statement::getType<Declaration>() const
{
    return *std::get<std::unique_ptr<Declaration>>(type);
}

template<typename T>
const T& Statement::getType() const
{
    return std::get<T>(type);
}

} // namespace CPPS::CST
