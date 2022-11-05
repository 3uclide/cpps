#include <catch2/catch_test_macros.hpp>

#include "cpps/source-reader.hpp"

namespace CPPS {

TEST_CASE("source_location fmt", "[source_location]")
{
    SourceReader sourceReader;

    SECTION("true")
    {
        CHECK(sourceReader.doSomething(true));
    }

    SECTION("false")
    {
        CHECK_FALSE(sourceReader.doSomething(false));
    }
}

} // namespace CPPS
