#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include "cpps/grammar/function-modifier.hpp"

namespace CPPS {

TEST_CASE("FunctionModifier format", "[FunctionModifier]")
{
    CHECK(fmt::format("{}", FunctionModifier::Final) == toStringView(FunctionModifier::Final));
}

TEST_CASE("FunctionModifier operator<< std::ostream", "[FunctionModifier]")
{
    std::stringstream stream;

    stream << FunctionModifier::Final;

    CHECK(stream.str() == toStringView(FunctionModifier::Final));
}

} // namespace CPPS
