#include "cpps/grammar/boolean-literal.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <ostream>

namespace CPPS {

TEST_CASE("BooleanLiteral format", "[BooleanLiteral]")
{
    CHECK(fmt::format("{}", BooleanLiteral::True) == toStringView(BooleanLiteral::True));
}

TEST_CASE("BooleanLiteral operator<< std::ostream", "[BooleanLiteral]")
{
    std::stringstream stream;

    stream << BooleanLiteral::True;

    CHECK(stream.str() == toStringView(BooleanLiteral::True));
}

} // namespace CPPS
