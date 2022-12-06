#include <catch2/catch_test_macros.hpp>

#include "cpps/grammar/character-literal.hpp"

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

} // namespace CPPS
