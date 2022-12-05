#pragma once

#include <string_view>

namespace CPPS {

class Diagnosis;

struct SourceLocation;

void checkNoWarning(const Diagnosis& diagnosis);
void checkNoError(const Diagnosis& diagnosis);
void checkNoErrorOrWarning(const Diagnosis& diagnosis);
void checkError(const Diagnosis& diagnosis, const std::string& msg, SourceLocation location);
void checkWarning(const Diagnosis& diagnosis, const std::string& msg, SourceLocation location);

} // namespace CPPS
