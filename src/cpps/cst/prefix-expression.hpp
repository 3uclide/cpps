#pragma once

#include <vector>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/postfix-expression.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

class PrefixExpression : public PostfixExpression
{
public:
    [[nodiscard]] constexpr const std::vector<TokenRef>& getOperators() const { return _operators; }

private:
    std::vector<TokenRef> _operators;
};

} // namespace CPPS::CST
