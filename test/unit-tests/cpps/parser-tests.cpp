#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include "cpps/check-diagnosis.hpp"
#include "cpps/diagnosis.hpp"
#include "cpps/lexer.hpp"
#include "cpps/parser.hpp"
#include "cpps/source.hpp"
#include "cpps/tokens.hpp"

namespace CPPS {

TEST_CASE("Parser", "[Parser]")
{
    Diagnosis diagnosis;
    Tokens tokens;
    Parser parser{diagnosis, tokens};
}

} // namespace CPPS
