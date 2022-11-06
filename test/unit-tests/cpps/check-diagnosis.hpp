#pragma once

namespace cpps {

class Diagnosis;

void checkNoWarning(const Diagnosis& diagnosis);
void checkNoError(const Diagnosis& diagnosis);
void checkNoErrorOrWarning(const Diagnosis& diagnosis);

} // namespace cpps
