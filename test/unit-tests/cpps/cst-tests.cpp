#include <catch2/catch_test_macros.hpp>

#include "cpps/cst.hpp"
#include "cpps/lexeme.hpp"
#include "cpps/source-location.hpp"
#include "cpps/token.hpp"
#include "cpps/utility/bump-pointer-allocator.hpp"

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
    BumpPointerAllocator<> allocator;

    TranslationUnit tu;

    {
        Node<Declaration> decl(allocator, UnqualifiedIdentifier{newToken(Lexeme{CPPS::Identifier{}}, "my_var")});

        decl->type = Node<FunctionSignature>(allocator);
        decl->initializer = Node<Statement>(allocator);
        decl->initializer.value()->type = Node<Declaration>(allocator, UnqualifiedIdentifier{newToken(Lexeme{CPPS::Identifier{}}, "i")});

        tu.declarations.add(std::move(decl));
    }

    {
        Node<Declaration> decl = Node<Declaration>(allocator, UnqualifiedIdentifier{newToken(Lexeme{CPPS::Identifier{}}, "my_var2")});

        decl->type = Node<IdentifierExpression>(allocator);
        decl->initializer = Node<Statement>(allocator);

        tu.declarations.add(std::move(decl));
    }

    CHECK(tu.declarations[0].type.is<FunctionSignature>());
    CHECK(tu.declarations[0].initializer.value()->type.is<Declaration>());
    CHECK(tu.declarations[1].type.is<IdentifierExpression>());
}

} // namespace CPPS::CST
