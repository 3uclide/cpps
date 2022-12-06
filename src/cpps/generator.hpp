#pragma once

#include <ostream>

namespace CPPS {

class Diagnosis;
class Tokens;
class Source;

namespace CST {

struct TranslationUnit;

} // namespace CST

class Generator
{
public:
    Generator(Diagnosis& diagnosis, const Source& source, const Tokens& tokens, const CST::TranslationUnit& tu, std::ostream& stream);

    void generate();

private:
    Diagnosis& _diagnosis;
    const Source& _source;
    const Tokens& _tokens;
    const CST::TranslationUnit& _tu;
    std::ostream& _stream;
};

} // namespace CPPS
