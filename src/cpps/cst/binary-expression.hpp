#pragma once

#include <functional>
#include <variant>
#include <vector>

#include "cpps/cst/.forward-declare-types.hpp"
#include "cpps/cst/prefix-expression.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token-ref.hpp"

namespace CPPS::CST {

template<typename ExpressionTypeT>
struct BinaryExpression : ExpressionTypeT
{
    struct Term : ExpressionTypeT
    {
        TokenRef op;
    };

    SourceLocation getLocation() const;

    template<typename T>
    requires(std::is_base_of_v<ExpressionTypeT, T>)
    [[nodiscard]] const std::vector<typename T::Term>& termsOf() const;

    template<typename T>
    requires(std::is_base_of_v<ExpressionTypeT, T>)
    [[nodiscard]] std::vector<typename T::Term>& termsOf();

    // cppcheck-suppress duplInheritedMember
    std::vector<Term> terms;
};

// clang-format off
struct IsAsExpression : BinaryExpression<PrefixExpression> {};
struct MultiplicativeExpression : BinaryExpression<IsAsExpression> {};
struct AdditiveExpression : BinaryExpression<MultiplicativeExpression> {};
struct ShiftExpression : BinaryExpression<AdditiveExpression> {};
struct CompareExpression : BinaryExpression<ShiftExpression> {};
struct RelationalExpression : BinaryExpression<CompareExpression> {};
struct EqualityExpression : BinaryExpression<RelationalExpression> {};
struct BitAndExpression : BinaryExpression<EqualityExpression> {};
struct BitXorExpression : BinaryExpression<BitAndExpression> {};
struct BitOrExpression : BinaryExpression<BitXorExpression> {};
struct LogicalAndExpression : BinaryExpression<BitOrExpression> {};
struct LogicalOrExpression : BinaryExpression<LogicalAndExpression> {};
struct AssignmentExpression : BinaryExpression<LogicalOrExpression> {};
// clang-format on

template<typename ExpressionTypeT>
SourceLocation BinaryExpression<ExpressionTypeT>::getLocation() const
{
    return ExpressionTypeT::getLocation();
}

template<typename ExpressionTypeT>
template<typename T>
requires(std::is_base_of_v<ExpressionTypeT, T>)
[[nodiscard]] const std::vector<typename T::Term>& BinaryExpression<ExpressionTypeT>::termsOf() const
{
    return ExpressionTypeT::terms;
}

template<typename ExpressionTypeT>
template<typename T>
requires(std::is_base_of_v<ExpressionTypeT, T> )
[[nodiscard]] std::vector<typename T::Term>& BinaryExpression<ExpressionTypeT>::termsOf()
{
    return T::terms;
}
} // namespace CPPS::CST
