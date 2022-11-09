#include "cpps/grammar/keyword.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

namespace CPPS {

TEST_CASE("Keyword format", "[Keyword]")
{
    CHECK(fmt::format("{}", Keyword::AlignAs) == toStringView(Keyword::AlignAs));
}

TEST_CASE("Keyword operator<< std::ostream", "[Keyword]")
{
    std::stringstream stream;

    stream << Keyword::AlignAs;

    CHECK(stream.str() == toStringView(Keyword::AlignAs));
}

} // namespace CPPS
