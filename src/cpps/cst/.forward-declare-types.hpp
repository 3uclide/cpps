#pragma once

namespace CPPS::CST {

struct AdditiveExpression;
struct AssignmentExpression;
struct BitAndExpression;
struct BitOrExpression;
struct BitXorExpression;
struct CompareExpression;
struct CompoundStatement;
struct Declaration;
struct DeclarationList;
struct EqualityExpression;
struct Expression;
struct ExpressionList;
struct ExpressionStatement;
struct FunctionSignature;
struct IdentifierExpression;
struct IsAsExpression;
struct IterationExpression;
struct IterationStatement;
struct LogicalAndExpression;
struct LogicalOrExpression;
struct MultiplicativeExpression;
struct ParameterDeclaration;
struct ParameterDeclarationList;
struct PostfixExpression;
struct PrefixExpression;
struct PrimaryExpression;
struct QualifiedIdentifier;
struct RelationalExpression;
struct ReturnStatement;
struct SelectionStatement;
struct ShiftExpression;
struct Statement;
struct StatementList;
struct TranslationUnit;
struct UnqualifiedIdentifier;

template<typename ExpressionTypeT>
struct BinaryExpression;

} // namespace CPPS::CST
