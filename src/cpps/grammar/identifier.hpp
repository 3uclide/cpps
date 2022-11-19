#pragma once

#include "cpps/grammar/character-literal.hpp"
#include "cpps/utility/strings.hpp"

namespace CPPS {

enum class Identifier
{
};

inline std::size_t findFirstIdentifierEnd(std::string_view line, std::size_t start_index = 0)
{
    // start with a letter or _
    const char start_c = line[start_index];
    if (!isAlphabetic(start_c) && start_c != '_')
    {
        return std::string_view::npos;
    }

    // next are letter, digit or _
    std::size_t index = findFirstNotOf(
        line,
        [](char c) { return isalnum(c) || c == '_'; },
        start_index + 1);

    // npos here means the end of the identifier
    if (index == std::string_view::npos)
    {
        return line.size();
    }

    return index;
}

} // namespace CPPS
