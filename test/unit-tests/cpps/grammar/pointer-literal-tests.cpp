#include "cpps/grammar/pointer-literal.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

namespace CPPS {

TEST_CASE("PointerLiteral format", "[PointerLiteral]")
{
    CHECK(fmt::format("{}", PointerLiteral::NullPtr) == toStringView(PointerLiteral::NullPtr));
}

TEST_CASE("PointerLiteral operator<< std::ostream", "[PointerLiteral]")
{
    std::stringstream stream;

    stream << PointerLiteral::NullPtr;

    CHECK(stream.str() == toStringView(PointerLiteral::NullPtr));
}

} // namespace CPPS
