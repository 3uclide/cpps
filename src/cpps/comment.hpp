#pragma once

#include "cpps/source-location.hpp"

#include <string>

namespace CPPS {

struct Comment
{
    enum class Type
    {
        Line, // comment
        Block /* comment */
    };

    std::string text;
    SourceLocation beginLocation;
    SourceLocation endLocation;
    Type type;
};

} // namespace CPPS
