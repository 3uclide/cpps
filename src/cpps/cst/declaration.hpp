#pragma once

#include <functional>
#include <memory>
#include <variant>

#include "cpps/cst/node-variant.hpp"
#include "cpps/cst/unqualified-identifier.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct Statement;
struct FunctionSignature;
struct IdentifierExpression;

struct Declaration
{
    using Type = NodeVariant<FunctionSignature, IdentifierExpression>;

    explicit Declaration(UnqualifiedIdentifier&& id);

    ~Declaration();

    [[nodiscard]] SourceLocation getLocation() const;

    UnqualifiedIdentifier identifier;

    Type type;

    std::optional<TokenRef> pointerDeclaration;

    std::optional<Node<Statement>> initializer;

    SourceLocation startLocation;
    SourceLocation endLocation;
    SourceLocation equalLocation;
};

inline Declaration::Declaration(UnqualifiedIdentifier&& id)
    : identifier(id)
{
}

inline SourceLocation Declaration::getLocation() const
{
    return startLocation;
}

} // namespace CPPS::CST
