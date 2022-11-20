#include "cpps/parser.hpp"

namespace CPPS {

Parser::Parser(Diagnosis& diagnosis, const Tokens& tokens)
    : _diagnosis(diagnosis)
    , _tokens(tokens)
{
}

std::optional<CST::TranslationUnit> Parser::parse()
{
    (void)_diagnosis;
    (void)_tokens;

    return std::move(_tu);
}

} // namespace CPPS
