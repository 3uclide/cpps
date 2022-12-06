#include <catch2/catch_test_macros.hpp>

#include "cpps/grammar/character-literal.hpp"
#include "cpps/utility/strings.hpp"

namespace CPPS {

TEST_CASE("findFirstNotOf", "[Utility]")
{
    CHECK(findFirstNotOf("0", isSpace, 0) == 0);
    CHECK(findFirstNotOf(" 1", isSpace, 0) == 1);
    CHECK(findFirstNotOf("  2", isSpace, 0) == 2);
    CHECK(findFirstNotOf("   3", isSpace, 0) == 3);

    CHECK(findFirstNotOf("01", isSpace, 1) == 1);
    CHECK(findFirstNotOf("0 2", isSpace, 1) == 2);
    CHECK(findFirstNotOf("0  3", isSpace, 1) == 3);
    CHECK(findFirstNotOf("0   4", isSpace, 1) == 4);
}

} // namespace CPPS
