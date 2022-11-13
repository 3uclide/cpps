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
class BinaryExpression : public ExpressionTypeT
{
public:
    struct Term : ExpressionTypeT
    {
        TokenRef identifier;
    };

public:
    [[nodiscard]] const std::vector<Term>& getTerms() const;
    [[nodiscard]] std::vector<Term>& getTerms();

    template<typename T>
    requires(std::is_base_of_v<ExpressionTypeT, T>)
    [[nodiscard]] const std::vector<typename T::Term>& getTermsOf() const;

    template<typename T>
    requires(std::is_base_of_v<ExpressionTypeT, T>)
    [[nodiscard]] std::vector<typename T::Term>& getTermsOf();

protected:
    // cppcheck-suppress duplInheritedMember
    std::vector<Term> _terms;
};

// clang-format off
class IsAsExpression : public BinaryExpression<PrefixExpression> {};
class MultiplicativeExpression : public BinaryExpression<IsAsExpression> {};
class AdditiveExpression : public BinaryExpression<MultiplicativeExpression> {};
class ShiftExpression : public BinaryExpression<AdditiveExpression> {};
class CompareExpression : public BinaryExpression<ShiftExpression> {};
class RelationalExpression : public BinaryExpression<CompareExpression> {};
class EqualityExpression : public BinaryExpression<RelationalExpression> {};
class BitAndExpression : public BinaryExpression<EqualityExpression> {};
class BitXorExpression : public BinaryExpression<BitAndExpression> {};
class BitOrExpression : public BinaryExpression<BitXorExpression> {};
class LogicalAndExpression : public BinaryExpression<BitOrExpression> {};
class LogicalOrExpression : public BinaryExpression<LogicalAndExpression> {};
class AssignmentExpression : public BinaryExpression<LogicalOrExpression> {};
// clang-format on

template<typename ExpressionTypeT>
const std::vector<typename BinaryExpression<ExpressionTypeT>::Term>& BinaryExpression<ExpressionTypeT>::getTerms() const
{
    return _terms;
}

template<typename ExpressionTypeT>
std::vector<typename BinaryExpression<ExpressionTypeT>::Term>& BinaryExpression<ExpressionTypeT>::getTerms()
{
    return _terms;
}

template<typename ExpressionTypeT>
template<typename T>
requires(std::is_base_of_v<ExpressionTypeT, T>)
[[nodiscard]] const std::vector<typename T::Term>& BinaryExpression<ExpressionTypeT>::getTermsOf() const
{
    const T& expression{*this};
    return expression.getTerms();
}

template<typename ExpressionTypeT>
template<typename T>
requires(std::is_base_of_v<ExpressionTypeT, T> )
[[nodiscard]] std::vector<typename T::Term>& BinaryExpression<ExpressionTypeT>::getTermsOf()
{
    T& expression{*this};
    return expression.getTerms();
}
} // namespace CPPS::CST
