#pragma once

#include <ostream>

namespace CPPS {

class Diagnosis;
class Tokens;
class Source;
class SourcePrinter;

namespace CST {

struct TranslationUnit;

} // namespace CST

class Generator
{
public:
    Generator(Diagnosis& diagnosis, const Source& source, const Tokens& tokens, const CST::TranslationUnit& tu, SourcePrinter& printer);

    void generate();

private:
    Diagnosis& _diagnosis;
    const Source& _source;
    const Tokens& _tokens;
    const CST::TranslationUnit& _tu;
    SourcePrinter& _printer;
};

} // namespace CPPS
