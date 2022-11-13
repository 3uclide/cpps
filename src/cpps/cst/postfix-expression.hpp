#pragma once

#include <memory>
#include <vector>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/expression-list.hpp"
#include "cpps/cst/identifier-expression.hpp"
#include "cpps/cst/primary-expression.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

class PostfixExpression
{
public:
    struct Term
    {
        TokenRef _operator;

        // This is used if *_operator is .
        std::optional<IdentifierExpression> _identifierExpression;

        // These is used if *_operator is [ or (
        std::optional<ExpressionList> _expressions;

        // This is used if *_operator is [ or (
        std::optional<TokenRef> _closeOperator;
    };

    [[nodiscard]] constexpr const std::vector<Term>& getTerms() const { return _terms; }

    std::unique_ptr<PrimaryExpression> expr;

    std::vector<Term> _terms;
};

} // namespace CPPS::CST
