#pragma once

#include "cpps/lexeme.hpp"
#include "cpps/source-location.hpp"

#include <string_view>

namespace CPPS {

struct Token
{
    Lexeme lexeme;
    SourceLocation location;
    std::string_view text;
};

} // namespace CPPS
