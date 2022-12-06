#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include "cpps/source-printer.hpp"

namespace CPPS {

TEST_CASE("SourcePrinter", "[SourcePrinter]")
{
    std::stringstream stream;
    SourcePrinter printer{stream};
    (void)printer;
}

} // namespace CPPS
