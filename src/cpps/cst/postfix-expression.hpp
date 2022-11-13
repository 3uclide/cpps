#pragma once

#include <memory>
#include <vector>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/expression-list.hpp"
#include "cpps/cst/identifier-expression.hpp"
#include "cpps/cst/primary-expression.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct PostfixExpression
{
    struct Term
    {
        TokenRef op;

        // This is used if *_operator is .
        std::optional<IdentifierExpression> identifierExpression;

        // These is used if *_operator is [ or (
        std::optional<ExpressionList> expressions;

        // This is used if *_operator is [ or (
        std::optional<TokenRef> closeOp;
    };

    std::unique_ptr<PrimaryExpression> expr;

    std::vector<Term> terms;
};

} // namespace CPPS::CST
