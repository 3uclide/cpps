#include <catch2/catch_test_macros.hpp>

#include "cpps/source-location.hpp"

namespace cpps {

TEST_CASE("SourceLocation default ctor to invalid", "[SourceLocation]")
{
    const SourceLocation location;

    CHECK(location == InvalidSourceLocation);
}

TEST_CASE("SourceLocation format", "[SourceLocation]")
{
    CHECK(fmt::format("{}", SourceLocation{.line = 42, .column = 24}) == "42, 24");
    CHECK(fmt::format("{}", SourceLocation{.line = 42, .column = InvalidSourceColumn}) == "42");
    CHECK(fmt::format("{}", InvalidSourceLocation).empty());
}

} // namespace cpps
