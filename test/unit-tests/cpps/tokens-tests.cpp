#include "cpps/tokens.hpp"

#include <catch2/catch_test_macros.hpp>

namespace CPPS {

TEST_CASE("Tokens", "[Tokens]")
{
    const Tokens tokens{[]() -> std::vector<Token> {
        std::vector<Token> tokens;

        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{0, 0}, ""});
        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{0, 1}, ""});
        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{0, 2}, ""});

        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{2, 0}, ""});
        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{2, 1}, ""});
        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{2, 2}, ""});

        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{3, 0}, ""});
        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{3, 1}, ""});

        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{4, 0}, ""});

        tokens.push_back({Lexeme(Lexeme::Slash), SourceLocation{6, 0}, ""});

        return tokens;
    }()};

    REQUIRE(tokens.size() == 10);
    REQUIRE(tokens.getLinesSize() == 7);

    CHECK(tokens[0].location == SourceLocation{0, 0});
    CHECK(tokens[1].location == SourceLocation{0, 1});
    CHECK(tokens[2].location == SourceLocation{0, 2});
    CHECK(tokens[3].location == SourceLocation{2, 0});
    CHECK(tokens[4].location == SourceLocation{2, 1});
    CHECK(tokens[5].location == SourceLocation{2, 2});
    CHECK(tokens[6].location == SourceLocation{3, 0});
    CHECK(tokens[7].location == SourceLocation{3, 1});
    CHECK(tokens[8].location == SourceLocation{4, 0});
    CHECK(tokens[9].location == SourceLocation{6, 0});

    REQUIRE(tokens.getLine(0).size() == 3);
    REQUIRE(tokens.getLine(1).size() == 0);
    REQUIRE(tokens.getLine(2).size() == 3);
    REQUIRE(tokens.getLine(3).size() == 2);
    REQUIRE(tokens.getLine(4).size() == 1);
    REQUIRE(tokens.getLine(5).size() == 0);
    REQUIRE(tokens.getLine(6).size() == 1);

    CHECK(tokens.getLine(0)[0].location == SourceLocation{0, 0});
    CHECK(tokens.getLine(0)[1].location == SourceLocation{0, 1});
    CHECK(tokens.getLine(0)[2].location == SourceLocation{0, 2});

    CHECK(tokens.getLine(2)[0].location == SourceLocation{2, 0});
    CHECK(tokens.getLine(2)[1].location == SourceLocation{2, 1});
    CHECK(tokens.getLine(2)[2].location == SourceLocation{2, 2});

    CHECK(tokens.getLine(3)[0].location == SourceLocation{3, 0});
    CHECK(tokens.getLine(3)[1].location == SourceLocation{3, 1});

    CHECK(tokens.getLine(4)[0].location == SourceLocation{4, 0});

    CHECK(tokens.getLine(6)[0].location == SourceLocation{6, 0});
}

TEST_CASE("Tokens default ctor", "[Tokens]")
{
    const Tokens tokens;

    CHECK(tokens.empty());
}

} // namespace CPPS
