#include "cpps/grammar/parameter-modifier.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <sstream>

namespace CPPS {

TEST_CASE("ParameterModifier format", "[ParameterModifier]")
{
    CHECK(fmt::format("{}", ParameterModifier::Copy) == toStringView(ParameterModifier::Copy));
}

TEST_CASE("ParameterModifier operator<< std::ostream", "[ParameterModifier]")
{
    std::stringstream stream;

    stream << ParameterModifier::Copy;

    CHECK(stream.str() == toStringView(ParameterModifier::Copy));
}

} // namespace CPPS
