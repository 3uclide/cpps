#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include "cpps/token.hpp"

namespace CPPS {

TEST_CASE("Token format", "[Token]")
{
    CHECK(fmt::format("{}", Token{Lexeme{BooleanLiteral::True}, SourceLocation{42, 24}, "text"}) == "[42, 24] true: 'text'");
    CHECK(fmt::format("{}", Token{Lexeme{Keyword::As}, SourceLocation{24, 42}, "text"}) == "[24, 42] as: 'text'");
}

TEST_CASE("Token operator<< std::ostream", "[Token]")
{
    std::stringstream stream;

    stream << Token{Lexeme{BooleanLiteral::True}, SourceLocation{42, 24}, "text"} << "\n";
    stream << Token{Lexeme{Keyword::As}, SourceLocation{24, 42}, "text"} << "\n";

    std::string expectedString =
        "[42, 24] true: 'text'\n"
        "[24, 42] as: 'text'\n";

    CHECK(stream.str() == expectedString);
}

} // namespace CPPS
