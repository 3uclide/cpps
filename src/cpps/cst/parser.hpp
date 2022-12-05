#pragma once

#include <string>

#include "cpps/cst.hpp"

namespace CPPS {

class Diagnosis;
class Tokens;

namespace CST {

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

    TranslationUnit parse();

private:
    Node<Declaration> parseDeclaration(bool mustEndWithSemicolon = true);

    Node<Declaration> parseUnnamedDeclaration(SourceLocation startLocation, bool mustEndWithSemicolon = true);
    Node<Declaration> parseUnnamedDeclaration(Node<UnqualifiedIdentifier>&& identifier, SourceLocation startLocation, bool mustEndWithSemicolon = true);

    bool parseParameterDeclarationList(ParameterDeclarationList& params, bool returns = false);
    Node<ParameterDeclaration> parseParameterDeclaration(bool returns = false);

private:
    Node<Expression> parseExpression(bool allowRelationalComparison = true);
    std::optional<ExpressionList> parseExpressionList(SourceLocation openParenthesisLocation);

    Node<PrefixExpression> parsePrefixExpression();
    Node<PostfixExpression> parsePostfixExpression();

    Node<PrimaryExpression> parsePrimaryExpression();
    Node<PrimaryExpression> parsePrimaryOpenParenthesisExpression();

    Node<IdentifierExpression> parseIdentifierExpression();
    Node<IterationStatement> parseIterationStatement();

private:
    template<typename BinaryExpressionT, typename... Args>
    bool parseBinaryExpression(BinaryExpressionT& binaryExpression, auto parseExpressionMethod, auto isValidTokenFunctor, Args... args);

    bool parseAssignmentExpression(AssignmentExpression& expr, bool allowRelationalComparison = true);
    bool parseAdditiveExpression(AdditiveExpression& expr);
    bool parseBasicExpression(BasicExpression& expr);
    bool parseBitAndExpression(BitAndExpression& expr, bool allowRelationalComparison = true);
    bool parseBitOrExpression(BitOrExpression& expr, bool allowRelationalComparison = true);
    bool parseBitXorExpression(BitXorExpression& expr, bool allowRelationalComparison = true);
    bool parseCompareExpression(CompareExpression& expr);
    bool parseEqualityExpression(EqualityExpression& expr, bool allowRelationalComparison = true);
    bool parseIsAsExpression(IsAsExpression& expr);
    bool parseLogicalAndExpression(LogicalAndExpression& expr, bool allowRelationalComparison = true);
    bool parseLogicalOrExpression(LogicalOrExpression& expr, bool allowRelationalComparison = true);
    bool parseMultiplicativeExpression(MultiplicativeExpression& expr);
    bool parseRelationalExpression(RelationalExpression& expr, bool allowRelationalComparison = true);
    bool parseShiftExpression(ShiftExpression& expr);

private:
    Node<Statement> parseStatement(bool mustEndWithSemicolon, SourceLocation equalLocation = {});

    Node<CompoundStatement> parseCompoundStatement(SourceLocation equalLocation);
    Node<ExpressionStatement> parseExpressionStatement(bool mustEndWithSemicolon = true);
    Node<ReturnStatement> parseReturnStatement();
    Node<SelectionStatement> parseSelectionStatement();

private:
    Node<FunctionSignature> parseFunctionSignature();
    Node<UnqualifiedIdentifier> parseUnqualifiedIdentifier();

private:
    [[nodiscard]] bool isEnd() const;

    [[nodiscard]] const Token& current() const;
    [[nodiscard]] const Token& peekBack(std::size_t offset) const;
    void next();

    void error(std::string message);
    void error(std::string message, SourceLocation location);

    [[nodiscard]] TranslationUnit::Allocator& allocator();

private:
    Diagnosis& _diagnosis;
    const Tokens& _tokens;

    TranslationUnit _tu;

    std::size_t _currentTokenIndex{0};
};

} // namespace CST
} // namespace CPPS
