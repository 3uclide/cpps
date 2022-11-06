#include <catch2/catch_test_macros.hpp>

#include "cpps/diagnosis.hpp"

namespace cpps {

TEST_CASE("Diagnosis", "[Diagnosis]")
{
    Diagnosis diagnosis;

    diagnosis.error("error(msg)");
    diagnosis.error("error(msg, 1)", 1);
    diagnosis.error("error(msg, {1, 1})", SourceLocation{.line = 1, .column = 1});
    diagnosis.error("error(msg, fixMsg)", "errorFix(msg, fixMsg)");
    diagnosis.error("error(msg, fixMsg, 1)", "errorFix(msg, fixMsg, 1)", 1);
    diagnosis.error("error(msg, fixMsg, {1, 1})", "errorFix(msg, fixMsg, {1, 1})", SourceLocation{.line = 1, .column = 1});

    diagnosis.warning("warning(msg)");
    diagnosis.warning("warning(msg, 1)", 1);
    diagnosis.warning("warning(msg, {1, 1})", SourceLocation{.line = 1, .column = 1});
    diagnosis.warning("warning(msg, fixMsg)", "warningFix(msg, fixMsg)");
    diagnosis.warning("warning(msg, fixMsg, 1)", "warningFix(msg, fixMsg, 1)", 1);
    diagnosis.warning("warning(msg, fixMsg, {1, 1})", "warningFix(msg, fixMsg, {1, 1})", SourceLocation{.line = 1, .column = 1});

    const std::span<const Diagnosis::Entry> errors = diagnosis.getErrors();
    const std::span<const Diagnosis::Entry> warnings = diagnosis.getWarnings();

    REQUIRE(errors.size() == 6);
    REQUIRE(warnings.size() == 6);

    CHECK(errors[0].message == "error(msg)");
    CHECK_FALSE(errors[0].fixMessage.has_value());
    CHECK_FALSE(errors[0].location.has_value());

    CHECK(errors[1].message == "error(msg, 1)");
    CHECK_FALSE(errors[1].fixMessage.has_value());
    REQUIRE(errors[1].location.has_value());
    CHECK(errors[1].location.value() == SourceLocation{1, InvalidSourceColumn});

    CHECK(errors[2].message == "error(msg, {1, 1})");
    CHECK_FALSE(errors[2].fixMessage.has_value());
    REQUIRE(errors[2].location.has_value());
    CHECK(errors[2].location.value() == SourceLocation{1, 1});

    CHECK(errors[3].message == "error(msg, fixMsg)");
    REQUIRE(errors[3].fixMessage.has_value());
    CHECK(errors[3].fixMessage.value() == "errorFix(msg, fixMsg)");
    CHECK_FALSE(errors[3].location.has_value());

    CHECK(errors[4].message == "error(msg, fixMsg, 1)");
    REQUIRE(errors[4].fixMessage.has_value());
    CHECK(errors[4].fixMessage.value() == "errorFix(msg, fixMsg, 1)");
    REQUIRE(errors[4].location.has_value());
    CHECK(errors[4].location.value() == SourceLocation{1, InvalidSourceColumn});

    CHECK(errors[5].message == "error(msg, fixMsg, {1, 1})");
    REQUIRE(errors[5].fixMessage.has_value());
    CHECK(errors[5].fixMessage.value() == "errorFix(msg, fixMsg, {1, 1})");
    REQUIRE(errors[5].location.has_value());
    CHECK(errors[5].location.value() == SourceLocation{1, 1});

    CHECK(warnings[0].message == "warning(msg)");
    CHECK_FALSE(warnings[0].fixMessage.has_value());
    CHECK_FALSE(warnings[0].location.has_value());

    CHECK(warnings[1].message == "warning(msg, 1)");
    CHECK_FALSE(warnings[1].fixMessage.has_value());
    REQUIRE(warnings[1].location.has_value());
    CHECK(warnings[1].location.value() == SourceLocation{1, InvalidSourceColumn});

    CHECK(warnings[2].message == "warning(msg, {1, 1})");
    CHECK_FALSE(warnings[2].fixMessage.has_value());
    REQUIRE(warnings[2].location.has_value());
    CHECK(warnings[2].location.value() == SourceLocation{1, 1});

    CHECK(warnings[3].message == "warning(msg, fixMsg)");
    REQUIRE(warnings[3].fixMessage.has_value());
    CHECK(warnings[3].fixMessage.value() == "warningFix(msg, fixMsg)");
    CHECK_FALSE(warnings[3].location.has_value());

    CHECK(warnings[4].message == "warning(msg, fixMsg, 1)");
    REQUIRE(warnings[4].fixMessage.has_value());
    CHECK(warnings[4].fixMessage.value() == "warningFix(msg, fixMsg, 1)");
    REQUIRE(warnings[4].location.has_value());
    CHECK(warnings[4].location.value() == SourceLocation{1, InvalidSourceColumn});

    CHECK(warnings[5].message == "warning(msg, fixMsg, {1, 1})");
    REQUIRE(warnings[5].fixMessage.has_value());
    CHECK(warnings[5].fixMessage.value() == "warningFix(msg, fixMsg, {1, 1})");
    REQUIRE(warnings[5].location.has_value());
    CHECK(warnings[5].location.value() == SourceLocation{1, 1});
}

} // namespace cpps
