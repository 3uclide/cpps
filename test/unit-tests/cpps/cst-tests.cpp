#include <catch2/catch_test_macros.hpp>

#include "cpps/cst/.include-types.hpp"
#include "cpps/lexeme.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token.hpp"

namespace CPPS::CST {

struct ConcreteSyntaxTreeFixture
{
    ConcreteSyntaxTreeFixture()
    {
        tokens.reserve(32);
    }

    const Token& newToken(Lexeme lexeme, std::string text)
    {
        return newToken(lexeme, SourceLocation{0, 0}, text);
    }

    const Token& newToken(Lexeme lexeme, SourceLocation location, std::string text)
    {
        assert(tokens.size() < tokens.capacity());
        tokens.emplace_back(lexeme, location, text);
        return tokens.back();
    }

    // must not realloc
    std::vector<Token> tokens;
};

TEST_CASE_METHOD(ConcreteSyntaxTreeFixture, "Concrete Syntax Tree Compilation", "[CST]")
{
    TranslationUnit tu;

    {
        std::unique_ptr decl = std::make_unique<Declaration>(UnqualifiedIdentifier{newToken(Lexeme{CPPS::Identifier{}}, "my_var")});

        decl->type = FunctionSignature();
        decl->initializer = std::make_unique<Statement>();
        decl->initializer->type = std::make_unique<Declaration>(UnqualifiedIdentifier{newToken(Lexeme{CPPS::Identifier{}}, "i")});

        tu.declarations.add(std::move(decl));
    }

    {
        std::unique_ptr decl = std::make_unique<Declaration>(UnqualifiedIdentifier{newToken(Lexeme{CPPS::Identifier{}}, "my_var2")});

        decl->type = IdentifierExpression();
        decl->initializer = std::make_unique<Statement>();

        tu.declarations.add(std::move(decl));
    }

    CHECK(tu.declarations[0].isType<FunctionSignature>());
    CHECK(tu.declarations[0].initializer->isType<Declaration>());
    CHECK(tu.declarations[1].isType<IdentifierExpression>());
}

} // namespace CPPS::CST
