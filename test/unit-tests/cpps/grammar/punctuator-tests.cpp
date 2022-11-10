#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include "cpps/grammar/punctuator.hpp"

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
