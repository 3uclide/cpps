#pragma once

#include <memory>
#include <variant>
#include <functional>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/function-signature.hpp"
#include "cpps/cst/identifier-expression.hpp"
#include "cpps/cst/statement.hpp"
#include "cpps/cst/unqualified-identifier.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

class Declaration
{
public:
    using Type = std::variant<FunctionSignature, IdentifierExpression>;

public:
    UnqualifiedIdentifier _identifier;

    Type _type;

    std::unique_ptr<Statement> _initializer;
    std::optional<TokenRef> _pointerDeclaration;

    SourceLocation _startLocation;
    SourceLocation _endLocation;
    SourceLocation _equalLocation;
};

} // namespace CPPS::CST
