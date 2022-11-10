#include "cpps/grammar/punctuator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <sstream>

namespace CPPS {

TEST_CASE("Punctuator format", "[Punctuator]")
{
    CHECK(fmt::format("{}", Punctuator::Ampersand) == toStringView(Punctuator::Ampersand));
}

TEST_CASE("Punctuator operator<< std::ostream", "[Punctuator]")
{
    std::stringstream stream;

    stream << Punctuator::Ampersand;

    CHECK(stream.str() == toStringView(Punctuator::Ampersand));
}

} // namespace CPPS
