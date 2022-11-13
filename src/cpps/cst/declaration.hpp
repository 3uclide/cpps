#pragma once

#include <functional>
#include <memory>
#include <variant>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/function-signature.hpp"
#include "cpps/cst/identifier-expression.hpp"
#include "cpps/cst/statement.hpp"
#include "cpps/cst/unqualified-identifier.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct Declaration
{
    using Type = std::variant<FunctionSignature, IdentifierExpression>;

    SourceLocation getLocation() const;

    template<typename T>
    bool isType() const;

    template<typename T>
    const T& getType() const;

    UnqualifiedIdentifier identifier;

    Type type;

    std::optional<TokenRef> pointerDeclaration;

    std::unique_ptr<Statement> initializer;

    SourceLocation startLocation;
    SourceLocation endLocation;
    SourceLocation equalLocation;
};

SourceLocation Declaration::getLocation() const
{
    return startLocation;
}

template<typename T>
bool Declaration::isType() const
{
    return std::holds_alternative<T>(type);
}

template<typename T>
const T& Declaration::getType() const
{
    return std::get<T>(type);
}

} // namespace CPPS::CST
