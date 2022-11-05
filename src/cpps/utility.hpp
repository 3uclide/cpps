#pragma once

#include <ctype.h>
#include <string_view>

namespace cpps {

// white-space:
inline bool isSpace(char c)
{
    return isspace(c);
}

// binary-digit: 0 1
inline bool isBinaryDigit(char c)
{
    return c == '0' || c == '1';
}

// digit: { 0..9 }
inline bool isDigit(char c)
{
    return isdigit(c);
}

// hexadecimal-digit: { 0..9 | a..z | A..F }
inline bool isHexadecimalDigit(char c)
{
    return isxdigit(c);
}

// alphabetic: { a..z | A..Z }
inline bool isAlphabetic(char c)
{
    return isalpha(c);
}

// alphanumeric: { 0..9 | a..z | A..Z }
inline bool isAlphanumeric(char c)
{
    return isalnum(c);
}

// nondigit: { a..z | A..Z | _ }
inline bool isNondigit(char c)
{
    return isAlphabetic(c) || c == '_';
}

// returns index of the first char not satisfying the predicate
std::size_t findFirstNotOf(std::string_view sv, auto predicate, std::size_t startIndex = 0)
{
    for (std::size_t index = startIndex; index < sv.size(); ++index)
    {
        if (!predicate(sv[index]))
        {
            return index;
        }
    }

    return std::string_view::npos;
}

} // namespace cpps
