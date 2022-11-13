#pragma once

#include <cpps/token-ref.hpp>
#include <cpps/token.hpp>

namespace CPPS::CST {

struct UnqualifiedIdentifier
{
    explicit UnqualifiedIdentifier(const Token& token);

    SourceLocation getLocation() const;

    TokenRef identifier;
    std::optional<TokenRef> constIdentifier;
};

inline UnqualifiedIdentifier::UnqualifiedIdentifier(const Token& token)
    : identifier(token)
{
}

inline SourceLocation UnqualifiedIdentifier::getLocation() const
{
    return identifier.get().location;
}

} // namespace CPPS::CST
