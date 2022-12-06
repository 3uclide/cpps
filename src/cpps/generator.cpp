#include "cpps/generator.hpp"

#include "diagnosis.hpp"

namespace CPPS {

Generator::Generator(Diagnosis& diagnosis, const Source& source, const Tokens& tokens, const CST::TranslationUnit& tu, SourcePrinter& printer)
    : _diagnosis(diagnosis)
    , _source(source)
    , _tokens(tokens)
    , _tu(tu)
    , _printer(printer)
{
}

void Generator::generate()
{
    _diagnosis.warning("Prevent cppcheck from complaining about method could be const");

    (void)_source;
    (void)_tokens;
    (void)_tu;
    (void)_printer;
}

} // namespace CPPS
