#pragma once

#include <istream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cpps/source-location.hpp"
#include "cpps/source.hpp"

namespace CPPS {

class Diagnosis;

class SourceReader
{
public:
    using Stream = std::istream;

    struct DiagnosisMessage
    {
        static std::string endNotFound();
        static std::string lineTooLong();
        static std::string unexpectedCloseBrace();
        static std::string unexpectedCharAfterCppsDefinition(char c);
        static std::string unreadableLine();
    };

    static constexpr std::size_t BufferCapacity{90ULL * 1024ULL};

public:
    SourceReader(Diagnosis& diagnosis, Stream& stream);

    std::optional<Source> read();

private:
    bool nextLine();

    bool readAsEmpty();
    bool readAsPreprocessor();
    bool readAsCpps();
    bool readAsImport();
    bool readAsCommentOrCpp();

    void setPreviousCommentAndEmptyLinesAsCpps();

    void addCurrentLine(Source::Line::Type type);

private:
    Diagnosis& _diagnosis;
    Stream& _stream;

    Source _source;

    std::vector<SourceLine> _braceDepth;

    std::string_view _rawLine;
    std::string_view _line;

    bool _isInComment : 1 {false};
    bool _isInStringLiteral : 1 {false};
};

} // namespace CPPS
