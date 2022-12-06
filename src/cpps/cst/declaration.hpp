#pragma once

#include <functional>
#include <memory>
#include <variant>

#include "cpps/cst/node.hpp"
#include "cpps/cst/node-variant.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct Statement;
struct FunctionSignature;
struct IdentifierExpression;
struct UnqualifiedIdentifier;

struct Declaration
{
    using Type = NodeVariant<FunctionSignature, IdentifierExpression>;

    ~Declaration();

    [[nodiscard]] SourceLocation getLocation() const;

    Node<UnqualifiedIdentifier> identifier;

    Type type;

    std::optional<TokenRef> pointerDeclaration;

    std::optional<Node<Statement>> initializer;

    SourceLocation startLocation;
    SourceLocation endLocation;
    SourceLocation equalLocation;
};

inline SourceLocation Declaration::getLocation() const
{
    return startLocation;
}

} // namespace CPPS::CST
