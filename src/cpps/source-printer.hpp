#pragma once

#include "source-location.hpp"

#include <ostream>

namespace CPPS {

class SourcePrinter
{
public:
    explicit SourcePrinter(std::ostream& stream);

    void print(std::string_view text, SourceLine line);

private:
    std::ostream& _stream;
};

} // namespace CPPS
