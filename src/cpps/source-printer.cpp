#include "cpps/source-printer.hpp"

namespace CPPS {

SourcePrinter::SourcePrinter(std::ostream& stream)
    : _stream(stream)
{
}

void SourcePrinter::print(std::string_view text, SourceLine line)
{
    (void)line;

    _stream << text;
}

} // namespace CPPS
