#include "cpps/check-diagnosis.hpp"

#include <algorithm>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include "cpps/diagnosis.hpp"

namespace CPPS {

void checkNoDiagnosis(std::span<const Diagnosis::Entry> entries, std::string_view type, std::string& msg)
{
    if (!entries.empty())
    {
        for (const auto& entry : entries)
        {
            if (entry.location.has_value())
            {
                msg += fmt::format("{} ({}): {}\n", type, *entry.location, entry.message);
            }
            else
            {
                msg += fmt::format("{}: {}\n", type, entry.message);
            }
        }
    }
}

void checkNoWarning(const Diagnosis& diagnosis)
{
    std::string msg;
    checkNoDiagnosis(diagnosis.getWarnings(), "warning", msg);

    if (!msg.empty())
    {
        FAIL(msg);
    }
}

void checkNoError(const Diagnosis& diagnosis)
{
    std::string msg;
    checkNoDiagnosis(diagnosis.getErrors(), "error", msg);

    if (!msg.empty())
    {
        FAIL(msg);
    }
}

void checkNoErrorOrWarning(const Diagnosis& diagnosis)
{
    std::string msg;
    checkNoDiagnosis(diagnosis.getErrors(), "error", msg);
    checkNoDiagnosis(diagnosis.getWarnings(), "warning", msg);

    if (!msg.empty())
    {
        FAIL(msg);
    }
}

void checkDiagnosis(std::span<const Diagnosis::Entry> entries, std::string_view type, std::string_view msg, SourceLocation location)
{
    REQUIRE_FALSE(entries.empty());

    auto it = std::find_if(entries.begin(), entries.end(), [&msg](const auto& entry) { return entry.message == msg; });

    if (it != entries.end())
    {
        CHECK(it->location == location);
    }
    else
    {
        FAIL(fmt::format("{} not reported: '{}'", type, msg));
    }
}

void checkError(const Diagnosis& diagnosis, const std::string& msg, SourceLocation location)
{
    checkDiagnosis(diagnosis.getErrors(), "error", msg, location);
}

void checkWarning(const Diagnosis& diagnosis, const std::string& msg, SourceLocation location)
{
    checkDiagnosis(diagnosis.getWarnings(), "warning", msg, location);
}

} // namespace CPPS
