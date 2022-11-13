#pragma once

#include <memory>
#include <vector>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/expression-list.hpp"
#include "cpps/cst/identifier-expression.hpp"
#include "cpps/cst/primary-expression.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct PostfixExpression : PrimaryExpression
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

    std::vector<Term> terms;
};

} // namespace CPPS::CST
