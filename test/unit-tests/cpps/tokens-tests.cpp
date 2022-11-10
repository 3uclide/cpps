#include <catch2/catch_test_macros.hpp>

#include "cpps/tokens.hpp"

namespace CPPS {

TEST_CASE("Tokens", "[Tokens]")
{
    const Tokens tokens{
        {{Lexeme(Punctuator::Slash), SourceLocation{0, 0}, ""},
         {Lexeme(Punctuator::Slash), SourceLocation{0, 1}, ""},
         {Lexeme(Punctuator::Slash), SourceLocation{0, 2}, ""},

         {Lexeme(Punctuator::Slash), SourceLocation{2, 0}, ""},
         {Lexeme(Punctuator::Slash), SourceLocation{2, 1}, ""},
         {Lexeme(Punctuator::Slash), SourceLocation{2, 2}, ""},

         {Lexeme(Punctuator::Slash), SourceLocation{3, 0}, ""},
         {Lexeme(Punctuator::Slash), SourceLocation{3, 1}, ""},
         {Lexeme(Punctuator::Slash), SourceLocation{4, 0}, ""},

         {Lexeme(Punctuator::Slash), SourceLocation{6, 0}, ""}},
        {}};

    REQUIRE(tokens.size() == 10);
    REQUIRE(tokens.lines() == 7);

    CHECK(tokens.at(0).location == SourceLocation{0, 0});
    CHECK(tokens.at(1).location == SourceLocation{0, 1});
    CHECK(tokens.at(2).location == SourceLocation{0, 2});
    CHECK(tokens.at(3).location == SourceLocation{2, 0});
    CHECK(tokens.at(4).location == SourceLocation{2, 1});
    CHECK(tokens.at(5).location == SourceLocation{2, 2});
    CHECK(tokens.at(6).location == SourceLocation{3, 0});
    CHECK(tokens.at(7).location == SourceLocation{3, 1});
    CHECK(tokens.at(8).location == SourceLocation{4, 0});
    CHECK(tokens.at(9).location == SourceLocation{6, 0});

    REQUIRE(tokens.size(0) == 3);
    REQUIRE(tokens.size(1) == 0);
    REQUIRE(tokens.size(2) == 3);
    REQUIRE(tokens.size(3) == 2);
    REQUIRE(tokens.size(4) == 1);
    REQUIRE(tokens.size(5) == 0);
    REQUIRE(tokens.size(6) == 1);

    CHECK(tokens.at(0, 0).location == SourceLocation{0, 0});
    CHECK(tokens.at(0, 1).location == SourceLocation{0, 1});
    CHECK(tokens.at(0, 2).location == SourceLocation{0, 2});

    CHECK(tokens.at(2, 0).location == SourceLocation{2, 0});
    CHECK(tokens.at(2, 1).location == SourceLocation{2, 1});
    CHECK(tokens.at(2, 2).location == SourceLocation{2, 2});

    CHECK(tokens.at(3, 0).location == SourceLocation{3, 0});
    CHECK(tokens.at(3, 1).location == SourceLocation{3, 1});

    CHECK(tokens.at(4, 0).location == SourceLocation{4, 0});

    CHECK(tokens.at(6, 0).location == SourceLocation{6, 0});
}

TEST_CASE("Tokens default ctor", "[Tokens]")
{
    const Tokens tokens;

    CHECK(tokens.empty());
    CHECK(tokens.comments().empty());
}

} // namespace CPPS
