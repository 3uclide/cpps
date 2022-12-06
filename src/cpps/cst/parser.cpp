#include "cpps/cst/parser.hpp"

#include <utility>

#include "cpps/diagnosis.hpp"
#include "cpps/tokens.hpp"

namespace CPPS::CST {

namespace {

template<Punctuator... PunctuatorsT>
constexpr bool anyTokenPunctuator(const Token& token)
{
    if (!token.lexeme.is<Punctuator>())
    {
        return false;
    }

    const Punctuator punctuator = token.lexeme.get<Punctuator>();

    return ((punctuator == PunctuatorsT) || ...);
}

template<Keyword... KeywordsT>
constexpr bool anyTokenKeyword(const Token& token)
{
    if (!token.lexeme.is<Keyword>())
    {
        return false;
    }

    const Keyword keyword = token.lexeme.get<Keyword>();

    return ((keyword == KeywordsT) || ...);
}

} // namespace

std::string Parser::DiagnosisMessage::dotMustFollowedByValidMemberName()
{
    return "'.' must be followed by a valid member name";
}

std::string Parser::DiagnosisMessage::illFormedInitializer()
{
    return "ill-formed initializer";
}

std::string Parser::DiagnosisMessage::invalidExpressionAfter(std::string_view token)
{
    return fmt::format("invalid expression after {}", token);
}

std::string Parser::DiagnosisMessage::invalidReturnParameterModifier(ParameterModifier modifier)
{
    return fmt::format("a return value cannot be '{}'", modifier);
}

std::string Parser::DiagnosisMessage::invalidReturnExpression()
{
    return "invalid return expression";
}

std::string Parser::DiagnosisMessage::invalidStatementInCompoundStatement()
{
    return "invalid statement in compound-statement";
}

std::string Parser::DiagnosisMessage::invalidTextInExpressionList()
{
    return "invalid text in expression list";
}

std::string Parser::DiagnosisMessage::missingCloseParenthesisForParameterList()
{
    return "missing close parenthesis in parameter list";
}

std::string Parser::DiagnosisMessage::missingCommaBetweenParameterDeclarations()
{
    return "missing comma between parameter declarations";
}

std::string Parser::DiagnosisMessage::missingEqualBeforeFunctionBody()
{
    return "missing = before function body";
}

std::string Parser::DiagnosisMessage::missingFunctionReturnAfterArrow()
{
    return "missing function return after ->";
}

std::string Parser::DiagnosisMessage::missingSemicolonAtEndDeclaration()
{
    return "missing ';' at end of the declaration";
}

std::string Parser::DiagnosisMessage::missingSemicolonAtEndStatement()
{
    return "missing ';' at the end of the statement";
}

std::string Parser::DiagnosisMessage::subscriptExpressionBracketEmpty()
{
    return "subscript expression [ ] must not be empty";
}

std::string Parser::DiagnosisMessage::unexpectedTextAfterExpressionList()
{
    return "unexpected text after expression-list";
}

std::string Parser::DiagnosisMessage::unexpectedTextAfterOpenParenthesis()
{
    return "unexpected text - ( is not followed by an expression-list";
}

std::string Parser::DiagnosisMessage::unexpectedTextBracketDoesNotMatch()
{
    return "unexpected text - [ is not properly matched by ]";
}

std::string Parser::DiagnosisMessage::unexpectedTextParenthesisDoesNotMatch()
{
    return "unexpected text - ( is not properly matched by )";
}

std::string Parser::DiagnosisMessage::unnamedDeclarationAtExpressionScopeMustBeFunction()
{
    return "an unnamed declaration at expression scope must be a function";
}

std::string Parser::DiagnosisMessage::unnamedFunctionAtExpressionScopeCannotReturnsMultipleValues()
{
    return "an unnamed function at expression scope currently cannot return multiple values";
}

Parser::Parser(Diagnosis& diagnosis, const Tokens& tokens)
    : _diagnosis(diagnosis)
    , _tokens(tokens)
{
}

// translation-unit:
//     declaration-seq
//          declaration
//          declaration_seq declaration
TranslationUnit Parser::parse()
{
    while (Node node = parseDeclaration())
    {
        _tu.declarations.add(std::move(node));
    }

    return std::move(_tu);
}

// declaration:
//    identifier unnamed-declaration
Node<Declaration> Parser::parseDeclaration(bool mustEndWithSemicolon)
{
    if (isEnd())
    {
        return {};
    }

    const std::size_t startIndex = _currentTokenIndex;
    const SourceLocation startLocation = current().location;

    if (Node identifier = parseUnqualifiedIdentifier())
    {
        if (Node declaration = parseUnnamedDeclaration(std::move(identifier), startLocation, mustEndWithSemicolon))
        {
            return declaration;
        }
        else
        {
            _currentTokenIndex = startIndex;
        }
    }

    return {};
}

// unnamed-declaration:
//      : function-signature = statement
//      : identifier-expression-opt = statement
//      : identifier-expression
Node<Declaration> Parser::parseUnnamedDeclaration(SourceLocation startLocation, bool mustEndWithSemicolon)
{
    if (current().lexeme != Punctuator::Colon)
    {
        // token must be :
        return {};
    }

    // skip the :
    next();

    Node<Declaration> declaration{allocator()};
    declaration->startLocation = startLocation;

    if (Node functionSignature = parseFunctionSignature())
    {
        declaration->type = std::move(functionSignature);
    }
    else if (current().lexeme == Punctuator::Multiply)
    {
        // pointer object
        declaration->pointerDeclaration = current();

        // skip the ptr decl
        next();

        declaration->type = parseIdentifierExpression();
    }
    else if (Node identifierExpression = parseIdentifierExpression())
    {
        // non-pointer object
        declaration->type = std::move(identifierExpression);
    }
    else
    {
        // TODO deduced type
        return {};
    }

    if (current().lexeme == Punctuator::Assignment)
    {
        declaration->equalLocation = current().location;

        // skip the =
        next();

        if (Node initializer = parseStatement(mustEndWithSemicolon, declaration->equalLocation))
        {
            declaration->initializer = std::move(initializer);
        }

        if (declaration->initializer == std::nullopt)
        {
            error(DiagnosisMessage::illFormedInitializer(), declaration->equalLocation);
            next();
            return {};
        }
    }
    else
    {
        if (declaration->type.is<FunctionSignature>())
        {
            error(DiagnosisMessage::missingEqualBeforeFunctionBody(), declaration->getLocation());
            return {};
        }

        if (current().lexeme == Punctuator::Semicolon)
        {
            // skip the ;
            next();
        }
        else if (mustEndWithSemicolon)
        {
            error(DiagnosisMessage::missingSemicolonAtEndDeclaration(), declaration->getLocation());
            return {};
        }
    }

    declaration->endLocation = peekBack(1).location;

    return declaration;
}

// unnamed-declaration:
//     : function-type = statement
//     : id-expression-opt = statement
//     : id-expression
Node<Declaration> Parser::parseUnnamedDeclaration(Node<UnqualifiedIdentifier>&& identifier, SourceLocation startLocation, bool mustEndWithSemicolon)
{
    if (Node declaration = parseUnnamedDeclaration(startLocation, mustEndWithSemicolon))
    {
        declaration->identifier = std::move(identifier);

        return declaration;
    }

    return {};
}

// parameter-declaration-list
//     ( parameter-declaration-seq-opt )
// parameter-declaration-seq:
//     parameter-declaration
//     parameter-declaration-seq , parameter-declaration
bool Parser::parseParameterDeclarationList(ParameterDeclarationList& params, bool returns)
{
    const Token& openParenthesisToken = current();

    if (openParenthesisToken.lexeme != Punctuator::OpenParenthesis)
    {
        return false;
    }

    params.openParenthesisLocation = openParenthesisToken.location;

    // skip the (
    next();

    while (Node param = parseParameterDeclaration(returns))
    {
        params.add(std::move(param));

        const Token& token = current();

        if (token.lexeme == Punctuator::CloseParenthesis)
        {
            break;
        }

        if (token.lexeme != Punctuator::Comma)
        {
            error(DiagnosisMessage::missingCommaBetweenParameterDeclarations(), token.location);
            return false;
        }

        // skip the ,
        next();
    }

    const Token& closeParenthesisToken = current();

    if (closeParenthesisToken.lexeme != Punctuator::CloseParenthesis)
    {
        error(DiagnosisMessage::missingCloseParenthesisForParameterList());
        next();
        return false;
    }

    params.closeParenthesisLocation = closeParenthesisToken.location;

    next();

    return true;
}

// parameter-declaration:
//     parameter-direction-opt declaration
// parameter-direction: one of
//     in copy inout out move forward
Node<ParameterDeclaration> Parser::parseParameterDeclaration(bool returns)
{
    const Token& startToken = current();

    ParameterModifier parameterModifier{returns ? ParameterModifier::Out : ParameterModifier::In};

    if (startToken.lexeme.is<ParameterModifier>())
    {
        switch (startToken.lexeme.get<ParameterModifier>())
        {
        case ParameterModifier::In:
        case ParameterModifier::InOut:
        case ParameterModifier::Copy:
        case ParameterModifier::Move:
            if (returns)
            {
                error(DiagnosisMessage::invalidReturnParameterModifier(startToken.lexeme.get<ParameterModifier>()), startToken.location);
                return {};
            }
            parameterModifier = startToken.lexeme.get<ParameterModifier>();
            next();
            break;

        case ParameterModifier::Out:
        case ParameterModifier::Forward:
            parameterModifier = startToken.lexeme.get<ParameterModifier>();
            next();
            break;
        }
    }

    Node decl = parseDeclaration(false);

    if (!decl)
    {
        return {};
    }

    Node<ParameterDeclaration> parameterDeclaration{allocator()};
    parameterDeclaration->declaration = std::move(decl);
    parameterDeclaration->modifier = parameterModifier;
    parameterDeclaration->location = startToken.location;
    return parameterDeclaration;
}

// expression:
//     assignment-expression
// TODO    try expression
Node<Expression> Parser::parseExpression(bool allowRelationalComparison)
{
    Node<AssignmentExpression> assignment{allocator()};

    if (!parseAssignmentExpression(*assignment, allowRelationalComparison))
    {
        return {};
    }

    Node<Expression> expression{allocator()};
    expression->assignment = std::move(assignment);
    return expression;
}

// expression-list:
//     expression
//     expression-list , expression
std::optional<ExpressionList> Parser::parseExpressionList(SourceLocation openParenthesisLocation)
{
    auto parseParameterModifier = [this]() {
        if (current().lexeme == ParameterModifier::Out)
        {
            next();
            return ParameterModifier::Out;
        }
        else if (current().lexeme == ParameterModifier::Move)
        {
            next();
            return ParameterModifier::Move;
        }
        return ParameterModifier::In;
    };

    ParameterModifier parameterModifier{parseParameterModifier()};

    ExpressionList expressions;
    expressions.openParenthesisLocation = openParenthesisLocation;

    // list is not empty
    if (Node firstExpression = parseExpression())
    {
        expressions.add(Node<ExpressionTerm>{allocator(), std::move(firstExpression), parameterModifier});

        while (current().lexeme == Punctuator::Comma)
        {
            // skip the ,
            next();

            parameterModifier = parseParameterModifier();

            if (Node nextExpression = parseExpression())
            {
                expressions.add(Node<ExpressionTerm>{allocator(), std::move(nextExpression), parameterModifier});
            }
            else
            {
                error(DiagnosisMessage::invalidTextInExpressionList());
                return {};
            }
        }
    }

    expressions.closeParenthesisLocation = current().location;

    return expressions;
}

// prefix-expression:
//     prefix-operator prefix-expression
// TODO await-expression
// TODO sizeof ( type-id )
// TODO sizeof ... ( identifier )
// TODO alignof ( type-id )
// TODO throws-expression
Node<PrefixExpression> Parser::parsePrefixExpression()
{
    auto isPrefixToken = [](const Token& token) {
        const Lexeme lexeme = token.lexeme;
        return lexeme == Punctuator::Not || lexeme == Punctuator::Plus || lexeme == Punctuator::Minus;
    };

    Node<PrefixExpression> prefix{allocator()};

    for (; isPrefixToken(current()); next())
    {
        prefix->ops.emplace_back(current());
    }

    return prefix;
}

// postfix-expression:
//     postfix-operator
//     [ expression-list ]
//     ( expression-list? )
//     . id-expression
Node<PostfixExpression> Parser::parsePostfixExpression()
{
    Node<PostfixExpression> postfix{allocator()};

    // TODO Find a better name
    auto shouldContinue = [this]() {
        auto isLexicallyAdjacent = [](const Token& current, const Token& previous) {
            return current.location.line == previous.location.line && current.location.column == previous.location.column + static_cast<SourceColumn>(previous.text.size());
        };

        const Token& curr = current();
        const Token& previous = peekBack(1);

        // clang-format off
        const bool isPostfix = curr.lexeme  == Punctuator::PlusPlus ||
                                curr.lexeme  == Punctuator::MinusMinus ||
                                curr.lexeme  == Punctuator::Multiply ||
                                curr.lexeme  == Punctuator::Ampersand ||
                                curr.lexeme  == Punctuator::Tilde ||
                                curr.lexeme  == Punctuator::Dollar;

        if ((!isPostfix || !isLexicallyAdjacent(curr , previous)) &&
            (curr.lexeme != Punctuator::OpenBracket && curr.lexeme != Punctuator::OpenParenthesis && curr.lexeme != Punctuator::Dot))
        {
            return false;
        }

        //  * & and ~ can't be a unary operator if followed by ( or identifier
        const bool is_invalid_sequence =
            (curr.lexeme == Punctuator::Multiply || curr.lexeme == Punctuator::Ampersand || curr.lexeme == Punctuator::Tilde) &&
            (curr.lexeme == Punctuator::OpenParenthesis || curr.lexeme.is<CPPS::Identifier>());
        // clang-format on

        return !is_invalid_sequence;
    };

    while (shouldContinue())
    {
        if (current().lexeme == Punctuator::Dollar)
        {
            // TODO handle $
            return {};
        }

        postfix->terms.emplace_back(current());
        PostfixExpression::Term& term{postfix->terms.back()};

        next();

        auto parseExpressionListForTerm = [this, &term](
                                              Punctuator closeLexeme,
                                              auto errMsgExpressionsNullptrGetter,
                                              auto errMsgNotMatchCloseGetter) {
            term.expressions = parseExpressionList(term.op.get().location);

            if (term.expressions == std::nullopt || (closeLexeme == Punctuator::CloseBracket && term.expressions->empty()))
            {
                error(errMsgExpressionsNullptrGetter(), term.op.get().location);
                return false;
            }

            const Token& closeToken = current();

            if (closeToken.lexeme != closeLexeme)
            {
                error(errMsgNotMatchCloseGetter());
                return false;
            }

            term.expressions.value().closeParenthesisLocation = closeToken.location;
            term.closeOp = closeToken;

            next();

            return true;
        };

        if (term.op.get().lexeme == Punctuator::OpenBracket)
        {
            if (!parseExpressionListForTerm(
                    Punctuator::CloseBracket,
                    &DiagnosisMessage::subscriptExpressionBracketEmpty,
                    &DiagnosisMessage::unexpectedTextBracketDoesNotMatch))
            {
                return {};
            }
        }
        else if (term.op.get().lexeme == Punctuator::OpenParenthesis)
        {
            if (!parseExpressionListForTerm(
                    Punctuator::CloseParenthesis,
                    &DiagnosisMessage::unexpectedTextAfterExpressionList,
                    &DiagnosisMessage::unexpectedTextParenthesisDoesNotMatch))
            {
                return {};
            }
        }
        else if (term.op.get().lexeme == Punctuator::Dot)
        {
            term.identifierExpression = parseIdentifierExpression();

            if (!term.identifierExpression)
            {
                error(DiagnosisMessage::dotMustFollowedByValidMemberName(), term.op.get().location);
                return {};
            }
        }
    }

    return postfix;
}

Node<PrimaryExpression> Parser::parsePrimaryExpression()
{
    if (Node id = parseIdentifierExpression())
    {
        Node<PrimaryExpression> node{allocator()};
        node->type = std::move(id);
        return node;
    }

    auto isLiteralOrIdentifierOrKeyword = [](const Token& token) {
        // clang-format off
        return token.lexeme.is<BinaryLiteral>() ||
               token.lexeme.is<BooleanLiteral>() ||
               token.lexeme.is<CharacterLiteral>() ||
               token.lexeme.is<DecimalLiteral>() ||
               token.lexeme.is<FloatingLiteral>() ||
               token.lexeme.is<CPPS::Identifier>() ||
               token.lexeme.is<Keyword>() ||
               token.lexeme.is<PointerLiteral>() ||
               token.lexeme.is<StringLiteral>();
        // clang-format on
    };

    if (isLiteralOrIdentifierOrKeyword(current()))
    {
        Node<PrimaryExpression> node{allocator()};
        node->type = std::ref(current());
        next();
        return node;
    }

    if (current().lexeme == Punctuator::OpenParenthesis)
    {
        return parsePrimaryOpenParenthesisExpression();
    }

    if (Node declaration = parseUnnamedDeclaration(current().location, true))
    {
        if (!declaration->type.is<FunctionSignature>())
        {
            error(DiagnosisMessage::unnamedDeclarationAtExpressionScopeMustBeFunction(), declaration->getLocation());

            next();

            return {};
        }

        const FunctionSignature& func = declaration->type.as<FunctionSignature>();

        if (func.returns.is<ParameterDeclarationList>())
        {
            error(DiagnosisMessage::unnamedFunctionAtExpressionScopeCannotReturnsMultipleValues(), func.returns.as<ParameterDeclarationList>().openParenthesisLocation);

            next();

            return {};
        }

        Node<PrimaryExpression> node{allocator()};
        node->type = std::move(declaration);
        return node;
    }

    return {};
}

Node<PrimaryExpression> Parser::parsePrimaryOpenParenthesisExpression()
{
    const SourceLocation openLocation = current().location;

    next();

    std::optional expressionsOpt = parseExpressionList(openLocation);

    if (!expressionsOpt.has_value())
    {
        error(DiagnosisMessage::unexpectedTextAfterOpenParenthesis());

        next();

        return {};
    }

    ExpressionList& expressions = expressionsOpt.value();

    if (current().lexeme != Punctuator::CloseParenthesis)
    {
        error(DiagnosisMessage::unexpectedTextAfterExpressionList());

        next();

        return {};
    }

    expressions.closeParenthesisLocation = current().location;

    next();

    Node<PrimaryExpression> primary{allocator()};
    primary->type = Node<ExpressionList>{allocator(), std::move(expressions)};
    return primary;
}

// id-expression
//    unqualified-id
//    qualified-id
Node<IdentifierExpression> Parser::parseIdentifierExpression()
{
    if (Node unqualifiedIdentifier = parseUnqualifiedIdentifier())
    {
        Node<IdentifierExpression> expression{allocator()};

        expression->location = current().location;
        expression->identifier.type = std::move(unqualifiedIdentifier);

        return expression;
    }
    return {};
}

Node<IterationStatement> Parser::parseIterationStatement()
{
    return {};
}

Node<FunctionSignature> Parser::parseFunctionSignature()
{
    ParameterDeclarationList parameters;
    if (!parseParameterDeclarationList(parameters))
    {
        return {};
    }

    // Optional "throws"
    bool throws = false;
    if (current().lexeme == Keyword::Throws)
    {
        throws = true;

        next();
    }

    // Optional returns
    FunctionSignature::Returns returns;
    if (current().lexeme == Punctuator::Arrow)
    {
        next();

        if (Node id_expr = parseIdentifierExpression())
        {
            returns = std::move(id_expr);
        }
        else
        {
            ParameterDeclarationList returnsParameters;
            if (parseParameterDeclarationList(returnsParameters, true))
            {
                Node<ParameterDeclarationList> node{allocator(), std::move(returnsParameters)};
                returns = std::move(node);
            }
            else
            {
                error(DiagnosisMessage::missingFunctionReturnAfterArrow());
                return {};
            }
        }
    }

    Node<FunctionSignature> functionSignature{allocator()};
    functionSignature->parameters = std::move(parameters);
    functionSignature->returns = std::move(returns);
    functionSignature->throws = throws;
    return functionSignature;
}

template<typename BinaryExpressionT, typename... Args>
bool Parser::parseBinaryExpression(BinaryExpressionT& binaryExpression, auto parseExpressionMethod, auto isValidTokenFunctor, Args... args)
{
    if (!(this->*parseExpressionMethod)(binaryExpression, args...))
    {
        return false;
    }

    while (isValidTokenFunctor(current()))
    {
        // add the token
        binaryExpression.terms.add(Node<typename BinaryExpressionT::Term>{allocator(), current()});

        // skip the operand
        next();

        // parse the expression
        if (!(this->*parseExpressionMethod)(binaryExpression.terms.back(), args...))
        {
            error(DiagnosisMessage::invalidExpressionAfter(binaryExpression.terms.back().op.get().text));
            return false;
        }
    }

    return true;
}

// additive-expression:
//     multiplicative-expression
//     additive-expression + multiplicative-expression
//     additive-expression - multiplicative-expression
bool Parser::parseAdditiveExpression(AdditiveExpression& expr)
{
    return parseBinaryExpression(expr, &Parser::parseMultiplicativeExpression, &anyTokenPunctuator<Punctuator::Plus, Punctuator::Minus>);
}

// basic-expression:
//     prefix-expression primary-expression postfix-expression
bool Parser::parseBasicExpression(BasicExpression& expr)
{
    expr.prefix = parsePrefixExpression();
    expr.primary = parsePrimaryExpression();

    if (!expr.primary)
    {
        return false;
    }

    expr.postfix = parsePostfixExpression();

    return true;
}

// assignment-expression:
//     logical-or-expression
//     assignment-expression assignment-operator assignment-expression
bool Parser::parseAssignmentExpression(AssignmentExpression& expr, bool allowRelationalComparison)
{
    // clang-format off
    return parseBinaryExpression(
        expr,
        &Parser::parseLogicalOrExpression,
        &anyTokenPunctuator<
            Punctuator::AmpersandEqual,
            Punctuator::Assignment,
            Punctuator::CaretEqual,
            Punctuator::LeftShiftEqual,
            Punctuator::MinusEqual,
            Punctuator::ModuloEqual,
            Punctuator::MultiplyEqual,
            Punctuator::PipeEqual,
            Punctuator::PlusEqual,
            Punctuator::RightShiftEqual,
            Punctuator::SlashEqual>,
        allowRelationalComparison);
    // clang-format on
}

// bit-and-expression:
//     equality-expression
//     bit-and-expression & equality-expression
bool Parser::parseBitAndExpression(BitAndExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseEqualityExpression,
        &anyTokenPunctuator<Punctuator::Ampersand>,
        allowRelationalComparison);
}

// bit-or-expression:
//     bit-xor-expression
//     bit-or-expression & bit-xor-expression
bool Parser::parseBitOrExpression(BitOrExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseBitXorExpression,
        &anyTokenPunctuator<Punctuator::Pipe>,
        allowRelationalComparison);
}

// bit-xor-expression:
//     bit-and-expression
//     bit-xor-expression & bit-and-expression
bool Parser::parseBitXorExpression(BitXorExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseBitAndExpression,
        &anyTokenPunctuator<Punctuator::Caret>,
        allowRelationalComparison);
}

// compare-expression:
//    shift-expression
//    compare-expression <=> shift-expression
bool Parser::parseCompareExpression(CompareExpression& expr)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseShiftExpression,
        &anyTokenPunctuator<Punctuator::Spaceship>);
}

// equality-expression:
//    relational-expression
//    equality-expression == relational-expression
//    equality-expression != relational-expression
bool Parser::parseEqualityExpression(EqualityExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseRelationalExpression,
        &anyTokenPunctuator<Punctuator::CompareEqual, Punctuator::CompareNotEqual>,
        allowRelationalComparison);
}

// is-as-expression:
//     basic-expression
// TODO    is-as-expression is-expression-constraint
// TODO    is-as-expression as-type-cast
// TODO    type-id is-type-constraint
bool Parser::parseIsAsExpression(IsAsExpression& expr)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseBasicExpression,
        &anyTokenKeyword<Keyword::As, Keyword::Is>);
}

// logical-and-expression:
//     bit-or-expression
//     logical-and-expression && bit-or-expression
bool Parser::parseLogicalAndExpression(LogicalAndExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseBitOrExpression,
        &anyTokenPunctuator<Punctuator::LogicalAnd>,
        allowRelationalComparison);
}

// constant-expression:    // don't need intermediate production, just use:
// conditional-expression: // don't need intermediate production, just use:
// logical-or-expression:
//     logical-and-expression
//     logical-or-expression || logical-and-expression
bool Parser::parseLogicalOrExpression(LogicalOrExpression& expr, bool allowRelationalComparison)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseLogicalAndExpression,
        &anyTokenPunctuator<Punctuator::LogicalOr>,
        allowRelationalComparison);
}

// multiplicative-expression:
//     is-as-expression
//     multiplicative-expression * is-as-expression
//     multiplicative-expression / is-as-expression
//     multiplicative-expression % is-as-expression
bool Parser::parseMultiplicativeExpression(MultiplicativeExpression& expr)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseIsAsExpression,
        &anyTokenPunctuator<Punctuator::Multiply, Punctuator::Slash, Punctuator::Modulo>);
}

// relational-expression:
//     compare-expression
//     relational-expression <  compare-expression
//     relational-expression >  compare-expression
//     relational-expression <= compare-expression
//     relational-expression >= compare-expression
bool Parser::parseRelationalExpression(RelationalExpression& expr, bool allowRelationalComparison)
{
    if (allowRelationalComparison)
    {
        return parseBinaryExpression(
            expr,
            &Parser::parseCompareExpression,
            &anyTokenPunctuator<Punctuator::Less, Punctuator::LessEqual, Punctuator::Greater, Punctuator::GreaterEqual>);
    }

    return parseBinaryExpression(
        expr,
        &Parser::parseCompareExpression,
        [](const Token&) { return false; });
}

// shift-expression:
//     additive-expression
//     shift-expression << additive-expression
//     shift-expression >> additive-expression
bool Parser::parseShiftExpression(ShiftExpression& expr)
{
    return parseBinaryExpression(
        expr,
        &Parser::parseAdditiveExpression,
        &anyTokenPunctuator<Punctuator::LeftShift, Punctuator::RightShift>);
}

// statement:
//     compound-statement
//     contract
//     declaration-statement
//     expression-statement
//     inspect-expression
//     iteration-statement
//     return-statement
//     selection-statement
//
// TODO     try-block
Node<Statement> Parser::parseStatement(bool mustEndWithSemicolon, SourceLocation equalLocation)
{
    auto makeStatementIfParseSucceed = [this]<typename... ArgsT>(auto parseStatementMethod, ArgsT&&... args) -> Node<Statement> {
        if (Node stmtType = (this->*parseStatementMethod)(std::forward<ArgsT>(args)...))
        {
            Node<Statement> stmt{allocator()};
            stmt->type = std::move(stmtType);
            return stmt;
        }
        return {};
    };

    if (Node stmt = makeStatementIfParseSucceed(&Parser::parseSelectionStatement)) return stmt;
    if (Node stmt = makeStatementIfParseSucceed(&Parser::parseReturnStatement)) return stmt;
    if (Node stmt = makeStatementIfParseSucceed(&Parser::parseIterationStatement)) return stmt;
    if (Node stmt = makeStatementIfParseSucceed(&Parser::parseCompoundStatement, equalLocation)) return stmt;
    if (Node stmt = makeStatementIfParseSucceed(&Parser::parseDeclaration, mustEndWithSemicolon)) return stmt;
    if (Node stmt = makeStatementIfParseSucceed(&Parser::parseExpressionStatement, mustEndWithSemicolon)) return stmt;

    return {};
}

// compound-statement:
//     { statement-seq-opt }
//
// statement-seq:
//     statement
//     statement-seq statement
Node<CompoundStatement> Parser::parseCompoundStatement(SourceLocation equalLocation)
{
    const Token& openBraceToken = current();

    if (openBraceToken.lexeme != Punctuator::OpenBrace)
    {
        return {};
    }

    const SourceLocation openBraceLocation = equalLocation.line == openBraceToken.location.line ? equalLocation : openBraceToken.location;

    // slip the {
    next();

    Node<CompoundStatement> statements{allocator()};

    statements->openBraceLocation = openBraceLocation;

    while (current().lexeme != Punctuator::CloseBrace)
    {
        static constexpr bool mustEndWithSemicolon = true;
        if (Node statement = parseStatement(mustEndWithSemicolon))
        {
            statements->add(std::move(statement));
        }
        else
        {
            error(DiagnosisMessage::invalidStatementInCompoundStatement());
            return {};
        }
    }

    statements->closeBraceLocation = current().location;

    // skip the }
    next();

    return statements;
}

// expression-statement:
//    expression ;
//    expression
Node<ExpressionStatement> Parser::parseExpressionStatement(bool mustEndWithSemicolon)
{
    Node expression = parseExpression();

    if (!expression)
    {
        return {};
    }

    bool hasSemicolon = false;

    if (current().lexeme == Punctuator::Semicolon)
    {
        hasSemicolon = true;

        // skip the ;
        next();
    }
    else if (mustEndWithSemicolon)
    {
        error(DiagnosisMessage::missingSemicolonAtEndStatement());
        return {};
    }

    Node<ExpressionStatement> exprStmt{allocator()};
    exprStmt->expression = std::move(expression);
    exprStmt->hasSemicolon = hasSemicolon;
    return exprStmt;
}

// return-statement:
//     return expression-opt ;
Node<ReturnStatement> Parser::parseReturnStatement()
{
    const Token& identifier = current();

    if (identifier.lexeme != Keyword::Return)
    {
        return {};
    }

    next();

    if (current().lexeme == Punctuator::Semicolon)
    {
        next();

        Node<ReturnStatement> returnStatement{allocator(), identifier};
        return returnStatement;
    }

    Node expression = parseExpression();

    if (!expression)
    {
        _diagnosis.error(DiagnosisMessage::invalidReturnExpression(), current().location);
        return {};
    }

    if (current().lexeme != Punctuator::Semicolon)
    {
        _diagnosis.error(DiagnosisMessage::missingSemicolonAtEndStatement(), current().location);

        next();

        return {};
    }

    next();

    Node<ReturnStatement> returnStatement{allocator(), identifier};
    returnStatement->expression = std::move(expression);
    return returnStatement;
}

// selection-statement:
//     if constexpr-opt expression compound-statement
//     if constexpr-opt expression compound-statement else compound-statement
Node<SelectionStatement> Parser::parseSelectionStatement()
{
    return {};
}

// unqualified-id:
//    const-opt identifier
//    const-opt template-id
// TODO operator-function-id
//
// template-id:
//    identifier < template-argument-list-opt >
//
// template-argument-list:
//    template-argument-list , template-argument
//
// template-argument:
//    expression
//    id-expression
//
Node<UnqualifiedIdentifier> Parser::parseUnqualifiedIdentifier()
{
    if (!current().lexeme.is<CPPS::Identifier>() && !current().lexeme.is<Keyword>())
    {
        return {};
    }

    Node<UnqualifiedIdentifier> identifier{allocator(), current()};

    next();

    if (current().lexeme == Punctuator::Less)
    {
        // TODO parse template arguments
        return {};
    }

    return identifier;
}

bool Parser::isEnd() const
{
    return _currentTokenIndex >= _tokens.size();
}

const Token& Parser::current() const
{
    return !isEnd() ? _tokens.at(_currentTokenIndex) : Token::invalid();
}

const Token& Parser::peekBack(std::size_t offset) const
{
    assert(_currentTokenIndex >= offset);

    const std::size_t i = _currentTokenIndex - offset;

    assert(i < _tokens.size());

    return _tokens.at(i);
}

void Parser::next()
{
    ++_currentTokenIndex;
}

void Parser::error(std::string message)
{
    error(std::move(message), isEnd() ? peekBack(1).location : current().location);
}

void Parser::error(std::string message, SourceLocation location)
{
    _diagnosis.error(std::move(message), location);
}

TranslationUnit::Allocator& Parser::allocator()
{
    return _tu.allocator;
}

} // namespace CPPS::CST
