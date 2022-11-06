#include "cpps/source-location.hpp"

#include <catch2/catch_test_macros.hpp>
#include <sstream>

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
    CHECK(fmt::format("{}", InvalidSourceLocation) == "invalid");
}

TEST_CASE("SourceLocation std::ostream", "[SourceLocation]")
{
    std::stringstream stream;

    stream << SourceLocation{0, 0} << "\n";
    stream << std::optional<SourceLocation>{{0, 0}} << "\n";
    stream << std::optional<SourceLocation>{} << "\n";

    std::string expectedString =
        "0, 0\n"
        "0, 0\n"
        "nullopt\n";

    CHECK(stream.str() == expectedString);
}

} // namespace cpps
