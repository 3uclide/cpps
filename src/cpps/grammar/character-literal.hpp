#pragma once

#include <ctype.h>

namespace CPPS {

enum class CharacterLiteral
{
};

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

} // namespace CPPS
