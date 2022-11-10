#include "cpps/check-diagnosis.hpp"

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

} // namespace CPPS
