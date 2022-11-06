#include "cpps/grammar/function-modifier.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <ostream>

namespace CPPS {

TEST_CASE("FunctionModifier format", "[Lexeme]")
{
    CHECK(fmt::format("{}", FunctionModifier::Final) == toStringView(FunctionModifier::Final));
}

TEST_CASE("FunctionModifier operator<< std::ostream", "[Lexeme]")
{
    std::stringstream stream;

    stream << FunctionModifier::Final;

    CHECK(stream.str() == toStringView(FunctionModifier::Final));
}

} // namespace CPPS
