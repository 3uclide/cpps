#include "cpps/utility.hpp"

#include <catch2/catch_test_macros.hpp>

namespace CPPS {

TEST_CASE("isSpace", "[Utility]")
{
    CHECK(isSpace(' '));
    CHECK_FALSE(isSpace('0'));
    CHECK_FALSE(isSpace('a'));
    CHECK_FALSE(isSpace('A'));
    CHECK_FALSE(isSpace('_'));
}

TEST_CASE("isBinaryDigit", "[Utility]")
{
    CHECK(isBinaryDigit('0'));
    CHECK(isBinaryDigit('1'));
    CHECK_FALSE(isBinaryDigit('2'));
    CHECK_FALSE(isBinaryDigit('a'));
    CHECK_FALSE(isBinaryDigit('A'));
    CHECK_FALSE(isBinaryDigit(' '));
    CHECK_FALSE(isBinaryDigit('_'));
}

TEST_CASE("isHexadecimalDigit", "[Utility]")
{
    for (char c = '0'; c <= '9'; ++c)
    {
        CHECK(isHexadecimalDigit(c));
    }

    for (char c = 'a'; c <= 'f'; ++c)
    {
        CHECK(isHexadecimalDigit(c));
    }

    for (char c = 'A'; c <= 'F'; ++c)
    {
        CHECK(isHexadecimalDigit(c));
    }

    CHECK_FALSE(isHexadecimalDigit('g'));
    CHECK_FALSE(isHexadecimalDigit('G'));
    CHECK_FALSE(isHexadecimalDigit(' '));
    CHECK_FALSE(isHexadecimalDigit('_'));
}

TEST_CASE("isAlphabetic", "[Utility]")
{
    for (char c = 'a'; c <= 'z'; ++c)
    {
        CHECK(isAlphabetic(c));
    }

    for (char c = 'A'; c <= 'Z'; ++c)
    {
        CHECK(isAlphabetic(c));
    }

    for (char c = '0'; c <= '9'; ++c)
    {
        CHECK_FALSE(isAlphabetic(c));
    }

    CHECK_FALSE(isAlphabetic(' '));
    CHECK_FALSE(isAlphabetic('_'));
}

TEST_CASE("isAlphanumeric", "[Utility]")
{
    for (char c = 'a'; c <= 'z'; ++c)
    {
        CHECK(isAlphanumeric(c));
    }

    for (char c = 'A'; c <= 'Z'; ++c)
    {
        CHECK(isAlphanumeric(c));
    }

    for (char c = '0'; c <= '9'; ++c)
    {
        CHECK(isAlphanumeric(c));
    }

    CHECK_FALSE(isAlphanumeric(' '));
    CHECK_FALSE(isAlphanumeric('_'));
}

TEST_CASE("isNondigit", "[Utility]")
{
    for (char c = 'a'; c <= 'z'; ++c)
    {
        CHECK(isNondigit(c));
    }

    for (char c = 'A'; c <= 'Z'; ++c)
    {
        CHECK(isNondigit(c));
    }

    CHECK(isNondigit('_'));

    for (char c = '0'; c <= '9'; ++c)
    {
        CHECK_FALSE(isNondigit(c));
    }

    CHECK_FALSE(isNondigit(' '));
}

TEST_CASE("findFirstNotOf", "[Utility]")
{
    CHECK(findFirstNotOf("0", isSpace, 0) == 0);
    CHECK(findFirstNotOf(" 1", isSpace, 0) == 1);
    CHECK(findFirstNotOf("  2", isSpace, 0) == 2);
    CHECK(findFirstNotOf("   3", isSpace, 0) == 3);

    CHECK(findFirstNotOf("01", isSpace, 1) == 1);
    CHECK(findFirstNotOf("0 2", isSpace, 1) == 2);
    CHECK(findFirstNotOf("0  3", isSpace, 1) == 3);
    CHECK(findFirstNotOf("0   4", isSpace, 1) == 4);
}

TEST_CASE("findFirstIdentifierEnd", "[Utility]")
{
    CHECK(findFirstIdentifierEnd("i") == 1);
    CHECK(findFirstIdentifierEnd("identifier: int;") == 10);
    CHECK(findFirstIdentifierEnd("_identifier: int;") == 11);

    CHECK(findFirstIdentifierEnd("0identifier: int;") == std::string_view::npos);
    CHECK(findFirstIdentifierEnd("-identifier: int;") == std::string_view::npos);
}

} // namespace CPPS
