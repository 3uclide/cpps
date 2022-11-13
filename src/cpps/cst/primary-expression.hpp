#pragma once

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/declaration.hpp"
#include "cpps/token-ref.hpp"
#include "cpps/token.hpp"

namespace CPPS::CST {

struct PrimaryExpression
{
    using Type = std::variant<
        std::monostate,
        std::unique_ptr<Declaration>,
        TokenRef,
        ExpressionList,
        IdentifierExpression>;

    template<typename T>
    [[nodiscard]] bool isType() const;

    template<typename T>
    const T& getType() const;

    [[nodiscard]] SourceLocation getLocation() const;

    Type type;
};

template<>
inline bool PrimaryExpression::isType<Declaration>() const
{
    return std::holds_alternative<std::unique_ptr<Declaration>>(type);
}

template<>
inline bool PrimaryExpression::isType<Token>() const
{
    return std::holds_alternative<TokenRef>(type);
}

template<typename T>
bool PrimaryExpression::isType() const
{
    return std::holds_alternative<T>(type);
}

template<>
inline const Declaration& PrimaryExpression::getType<Declaration>() const
{
    return *std::get<std::unique_ptr<Declaration>>(type);
}

template<>
inline const Token& PrimaryExpression::getType<Token>() const
{
    return std::get<TokenRef>(type).get();
}

template<typename T>
const T& PrimaryExpression::getType() const
{
    return std::get<T>(type);
}

inline SourceLocation PrimaryExpression::getLocation() const
{
    if (isType<Token>()) { return getType<Token>().location; }
    if (isType<ExpressionList>()) { return getType<ExpressionList>().getLocation(); }
    if (isType<IdentifierExpression>()) { return getType<IdentifierExpression>().getLocation(); }
    if (isType<Declaration>()) { return getType<Declaration>().getLocation(); }

    assert(std::holds_alternative<std::monostate>(type));

    return {0, 0};
}

} // namespace CPPS::CST
