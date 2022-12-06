#pragma once

#include <vector>

#include "cpps/cst/node-list.hpp"
#include "cpps/cst/parameter-modifier.hpp"

namespace CPPS::CST {

struct Expression;

struct ExpressionTerm
{
    ExpressionTerm() = default;
    ExpressionTerm(ExpressionTerm&&) = default;
    ExpressionTerm& operator=(ExpressionTerm&&) = default;

    ExpressionTerm(const ExpressionTerm&) = delete;
    ExpressionTerm& operator=(const ExpressionTerm&) = delete;

    ExpressionTerm(Node<Expression>&& expr, ParameterModifier mod);
    ~ExpressionTerm();

    SourceLocation getLocation() const;

    Node<Expression> expression;
    ParameterModifier modifier{ParameterModifier::In};
};

struct ExpressionList : NodeList<ExpressionTerm>
{
    using NodeList<ExpressionTerm>::NodeList;

    ExpressionList() = default;
    ExpressionList(ExpressionList&&) = default;
    ExpressionList& operator=(ExpressionList&&) = default;

    ExpressionList(const ExpressionList&) = delete;
    ExpressionList& operator=(const ExpressionList&) = delete;

    ~ExpressionList();

    SourceLocation openParenthesisLocation;
    SourceLocation closeParenthesisLocation;
};

} // namespace CPPS::CST
