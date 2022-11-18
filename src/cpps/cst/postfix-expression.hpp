#pragma once

#include <optional>
#include <vector>

#include "cpps/cst/expression-list.hpp"
#include "cpps/cst/node.hpp"
#include "cpps/cst/primary-expression.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

struct IdentifierExpression;

struct PostfixExpression
{
    struct Term
    {
        TokenRef op;

        // This is used if *_operator is .
        Node<IdentifierExpression> identifierExpression;

        // These is used if *_operator is [ or (
        std::optional<ExpressionList> expressions;

        // This is used if *_operator is [ or (
        std::optional<TokenRef> closeOp;
    };

    std::vector<Term> terms;
};

} // namespace CPPS::CST
