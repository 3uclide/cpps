#pragma once

#include <string_view>

namespace CPPS {

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

} // namespace CPPS
