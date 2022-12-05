#pragma once

#include <string>

#include "cpps/cst.hpp"

namespace CPPS::CST {

class Diagnosis;
class Tokens;

class Parser
{
public:
    struct DiagnosisMessage
    {
        static std::string dotMustFollowedByValidMemberName();
        static std::string illFormedInitializer();
        static std::string invalidExpressionAfter(std::string_view token);
        static std::string invalidReturnExpression();
        static std::string invalidReturnParameterModifier(ParameterModifier modifier);
        static std::string invalidStatementInCompoundStatement();
        static std::string invalidTextInExpressionList();
        static std::string missingCloseParenthesisForParameterList();
        static std::string missingCommaBetweenParameterDeclarations();
        static std::string missingEqualBeforeFunctionBody();
        static std::string missingFunctionReturnAfterArrow();
        static std::string missingSemicolonAtEndDeclaration();
        static std::string missingSemicolonAtEndStatement();
        static std::string subscriptExpressionBracketEmpty();
        static std::string unexpectedTextAfterExpressionList();
        static std::string unexpectedTextAfterOpenParenthesis();
        static std::string unexpectedTextBracketDoesNotMatch();
        static std::string unexpectedTextParenthesisDoesNotMatch();
        static std::string unnamedDeclarationAtExpressionScopeMustBeFunction();
        static std::string unnamedFunctionAtExpressionScopeCannotReturnsMultipleValues();
    };

public:
    Parser(Diagnosis& diagnosis, const Tokens& tokens);

    CST::TranslationUnit parse();

private:
    CST::Node<CST::Declaration> parseDeclaration(bool mustEndWithSemicolon = true);

    CST::Node<CST::Declaration> parseUnnamedDeclaration(SourceLocation startLocation, bool mustEndWithSemicolon = true);
    CST::Node<CST::Declaration> parseUnnamedDeclaration(CST::Node<CST::UnqualifiedIdentifier>&& identifier, SourceLocation startLocation, bool mustEndWithSemicolon = true);

    bool parseParameterDeclarationList(CST::ParameterDeclarationList& params, bool returns = false);
    CST::Node<CST::ParameterDeclaration> parseParameterDeclaration(bool returns = false);

private:
    CST::Node<CST::Expression> parseExpression(bool allowRelationalComparison = true);
    std::optional<CST::ExpressionList> parseExpressionList(SourceLocation openParenthesisLocation);

    CST::Node<CST::PrefixExpression> parsePrefixExpression();
    CST::Node<CST::PostfixExpression> parsePostfixExpression();

    CST::Node<CST::PrimaryExpression> parsePrimaryExpression();
    CST::Node<CST::PrimaryExpression> parsePrimaryOpenParenthesisExpression();

    CST::Node<CST::IdentifierExpression> parseIdentifierExpression();
    CST::Node<CST::IterationStatement> parseIterationStatement();

private:
    template<typename BinaryExpressionT, typename... Args>
    bool parseBinaryExpression(BinaryExpressionT& binaryExpression, auto parseExpressionMethod, auto isValidTokenFunctor, Args... args);

    bool parseAssignmentExpression(CST::AssignmentExpression& expr, bool allowRelationalComparison = true);
    bool parseAdditiveExpression(CST::AdditiveExpression& expr);
    bool parseBasicExpression(CST::BasicExpression& expr);
    bool parseBitAndExpression(CST::BitAndExpression& expr, bool allowRelationalComparison = true);
    bool parseBitOrExpression(CST::BitOrExpression& expr, bool allowRelationalComparison = true);
    bool parseBitXorExpression(CST::BitXorExpression& expr, bool allowRelationalComparison = true);
    bool parseCompareExpression(CST::CompareExpression& expr);
    bool parseEqualityExpression(CST::EqualityExpression& expr, bool allowRelationalComparison = true);
    bool parseIsAsExpression(CST::IsAsExpression& expr);
    bool parseLogicalAndExpression(CST::LogicalAndExpression& expr, bool allowRelationalComparison = true);
    bool parseLogicalOrExpression(CST::LogicalOrExpression& expr, bool allowRelationalComparison = true);
    bool parseMultiplicativeExpression(CST::MultiplicativeExpression& expr);
    bool parseRelationalExpression(CST::RelationalExpression& expr, bool allowRelationalComparison = true);
    bool parseShiftExpression(CST::ShiftExpression& expr);

private:
    CST::Node<CST::Statement> parseStatement(bool mustEndWithSemicolon, SourceLocation equalLocation = {});

    CST::Node<CST::CompoundStatement> parseCompoundStatement(SourceLocation equalLocation);
    CST::Node<CST::ExpressionStatement> parseExpressionStatement(bool mustEndWithSemicolon = true);
    CST::Node<CST::ReturnStatement> parseReturnStatement();
    CST::Node<CST::SelectionStatement> parseSelectionStatement();

private:
    CST::Node<CST::FunctionSignature> parseFunctionSignature();
    CST::Node<CST::UnqualifiedIdentifier> parseUnqualifiedIdentifier();

private:
    [[nodiscard]] bool isEnd() const;

    [[nodiscard]] const Token& current() const;
    [[nodiscard]] const Token& peekBack(std::size_t offset) const;
    void next();

    void error(std::string message);
    void error(std::string message, SourceLocation location);

    [[nodiscard]] CST::TranslationUnit::Allocator& allocator();

private:
    Diagnosis& _diagnosis;
    const Tokens& _tokens;

    CST::TranslationUnit _tu;

    std::size_t _currentTokenIndex{0};
};

} // namespace CPPS
