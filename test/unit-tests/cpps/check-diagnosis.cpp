#include "cpps/check-diagnosis.hpp"

#include "cpps/diagnosis.hpp"

#include <catch2/catch_test_macros.hpp>

namespace cpps {

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

}
