#pragma once

#include <optional>

#include "cpps/cst.hpp"

namespace CPPS {

class Diagnosis;
class Tokens;

class Parser
{
public:
    Parser(Diagnosis& diagnosis, const Tokens& tokens);

    std::optional<CST::TranslationUnit> parse();

private:
    Diagnosis& _diagnosis;
    const Tokens& _tokens;

    CST::TranslationUnit _tu;
};

} // namespace CPPS
