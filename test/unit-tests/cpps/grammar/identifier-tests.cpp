#include <catch2/catch_test_macros.hpp>

#include "cpps/grammar/identifier.hpp"

namespace CPPS {

TEST_CASE("findFirstIdentifierEnd", "[Utility]")
{
    CHECK(findFirstIdentifierEnd("i") == 1);
    CHECK(findFirstIdentifierEnd("identifier: int;") == 10);
    CHECK(findFirstIdentifierEnd("_identifier: int;") == 11);

    CHECK(findFirstIdentifierEnd("0identifier: int;") == std::string_view::npos);
    CHECK(findFirstIdentifierEnd("-identifier: int;") == std::string_view::npos);
}

} // namespace CPPS
