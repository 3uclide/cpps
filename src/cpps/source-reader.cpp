#include "cpps/source-reader.hpp"

#include "cpps/diagnosis.hpp"
#include "cpps/utility.hpp"

#include <range/v3/view.hpp>
#include <array>

namespace cpps {

namespace Details {

bool startsWithIdentifierColon(std::string_view line)
{
    std::size_t i = findFirstIdentifierEnd(line);
    if (i == std::string_view::npos)
    {
        return false;
    }

    // skip spaces
    i = findFirstNotOf(line, isspace, i);
    if (i == std::string_view::npos)
    {
        return false;
    }

    // next is ':' and not followed by another ':'
    return line[i] == ':' && (i == line.size() - 1 || line[i + 1] != ':');
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
thread_local std::array<char, SourceReader::BufferCapacity> Buffer{'\0'};

} // namespace Details

std::string SourceReader::DiagnosisMessage::endNotFound()
{
    return "source end not found";
}

std::string SourceReader::DiagnosisMessage::lineTooLong()
{
    return fmt::format("source line is too long (maximum length is {})", Details::Buffer.size());
}

std::string SourceReader::DiagnosisMessage::unexpectedCloseBrace()
{
    return "unexpected }";
}

std::string SourceReader::DiagnosisMessage::unexpectedCharAfterCppsDefinition(char c)
{
    return fmt::format("unexpected char '{}'- after cpps definition closing ; or }}", c);
}

std::string SourceReader::DiagnosisMessage::unreadableLine()
{
    return "source line is unreadable";
}

SourceReader::SourceReader(Diagnosis& diagnosis, Stream& stream)
    : _stream(stream)
    , _diagnosis(diagnosis)
{
}

std::optional<Source> SourceReader::read()
{
    while (nextLine())
    {
        const bool succeeded = readAsEmpty() || readAsPreprocessor() || readAsCpps() || readAsImport() || readAsCommentOrCpp();

        if (!succeeded)
        {
            // line should at least be read as comment or cpp
            _diagnosis.error(DiagnosisMessage::unreadableLine(), _source.size() - 1);
        }
    }

    const std::size_t previousLineSize = static_cast<std::size_t>(_stream.gcount());

    if (previousLineSize >= Details::Buffer.size() - 1U)
    {
        _diagnosis.error(DiagnosisMessage::lineTooLong(), _source.size());
        return std::nullopt;
    }

    if (!_stream.eof())
    {
        _diagnosis.error(DiagnosisMessage::endNotFound(), _source.size());
        return std::nullopt;
    }

    return std::move(_source);
}

bool SourceReader::nextLine()
{
    if (_stream.getline(Details::Buffer.data(), static_cast<std::streamsize>(Details::Buffer.size())))
    {
        _rawLine = {Details::Buffer.data()};

        const std::size_t i = findFirstNotOf(_rawLine, isSpace);

        _line = i != std::string_view::npos ? _rawLine.substr(i) : std::string_view{};

        return true;
    }

    _rawLine = {};
    _line = {};

    return false;
}

bool SourceReader::readAsEmpty()
{
    if (!_line.empty())
    {
        return false;
    }

    addCurrentLine(Source::Line::Type::Empty);
    return true;
}

bool SourceReader::readAsPreprocessor()
{
    if (_line[0] != '#')
    {
        return false;
    }

    addCurrentLine(Source::Line::Type::Preprocessor);

    // TODO Trailing spaces
    while (_line.back() == '\\' && nextLine())
    {
        addCurrentLine(Source::Line::Type::Preprocessor);
    }

    return true;
}

bool SourceReader::readAsCpps()
{
    if (_isInComment)
    {
        return false;
    }

    if (!_braceDepth.empty())
    {
        return false;
    }

    if (!Details::startsWithIdentifierColon(_line))
    {
        return false;
    }

    addCurrentLine(Source::Line::Type::Cpps);

    setPreviousCommentAndEmptyLinesAsCpps();

    bool foundEnd{false};
    bool hasNewLine{true};

    do
    {
        char previous{' '};
        for (std::size_t i = 0; i < _line.size(); ++i)
        {
            const char current = _line[i];

            if (_isInComment)
            {
                if (previous == '*' && current == '/')
                {
                    _isInComment = false;
                }
            }
            else
            {
                if (foundEnd && !isSpace(current))
                {
                    _diagnosis.error(DiagnosisMessage::unexpectedCharAfterCppsDefinition(current), SourceLocation{.line = _source.size() - 1, .column = static_cast<SourceColumn>(i)});
                }

                switch (current)
                {
                case '{':
                    _braceDepth.push_back(_source.size());
                    break;

                case '}':
                    if (_braceDepth.empty())
                    {
                        _diagnosis.error(DiagnosisMessage::unexpectedCloseBrace(), SourceLocation{.line = _source.size() - 1, .column = static_cast<SourceColumn>(i)});
                    }
                    else
                    {
                        _braceDepth.pop_back();

                        if (_braceDepth.empty())
                        {
                            foundEnd = true;
                        }
                    }
                    break;

                case ';':
                    if (_braceDepth.empty())
                    {
                        foundEnd = true;
                    }
                    break;

                case '*':
                    if (previous == '/')
                    {
                        _isInComment = true;
                    }
                    break;

                case '/':
                    if (previous == '/')
                    {
                        _isInComment = false;

                        i = _line.size(); // it's a comment line, we can stop here
                    }
                    break;
                }
            }

            previous = current;
        }

        if (!foundEnd)
        {
            hasNewLine = nextLine();

            if (hasNewLine)
            {
                addCurrentLine(Source::Line::Type::Cpps);
            }
        }
    } while (!foundEnd && hasNewLine);

    return foundEnd;
}

bool SourceReader::readAsImport()
{
    if (!_line.starts_with("import"))
    {
        return false;
    }

    addCurrentLine(Source::Line::Type::Import);

    return true;
}

bool SourceReader::readAsCommentOrCpp()
{
    if (_line.empty())
    {
        return false;
    }

    std::size_t i = 0;

    const bool is_start_of_comment = !_isInComment && !_isInStringLiteral && _line[0] == '/';

    if (is_start_of_comment && _line.size() >= 2)
    {
        if (_line[1] == '/')
        {
            addCurrentLine(Source::Line::Type::Comment);
            return true;
        }
        else if (_line[1] == '*')
        {
            _isInComment = true;

            i = _line.find("*/", 1);

            // comment continue on next line
            if (i == std::string_view::npos)
            {
                addCurrentLine(Source::Line::Type::Comment);
                return true;
            }

            i += 3; // advance after */

            _isInComment = false;
        }
    }

    bool isCommentLineOnly{true};
    char previous{' '};
    for (; i < _line.size(); ++i)
    {
        const char current = _line[i];

        if (_isInComment && !_isInStringLiteral)
        {
            if (current == '/' && previous == '*')
            {
                _isInComment = false;
            }
        }
        else
        {
            isCommentLineOnly = false;

            auto is_current_single_quoted = [this, previous, i]() {
                const std::size_t next_i = i + 1;
                return next_i < _line.size() && previous == '\'' && _line[next_i] == '\'';
            };

            switch (current)
            {
            case '\"':
                //  If this isn't an escaped quote
                if (!_isInComment && previous != '\\' && (_isInStringLiteral || previous != '\''))
                {
                    _isInStringLiteral = !_isInStringLiteral;
                }
                break;

            case '{':
                if (!_isInStringLiteral && !is_current_single_quoted())
                {
                    _braceDepth.push_back(static_cast<SourceLine>(i));
                }
                break;

            case '}':
                if (!_isInStringLiteral && !is_current_single_quoted())
                {
                    if (_braceDepth.empty())
                    {
                        _diagnosis.error(DiagnosisMessage::unexpectedCloseBrace(), SourceLocation{.line = _source.size(), .column = static_cast<SourceColumn>(i)});
                    }
                    else
                    {
                        _braceDepth.pop_back();
                    }
                }
                break;

            case '*':
                if (!_isInStringLiteral && previous == '/')
                {
                    _isInComment = true;
                }
                break;

            case '/':
                if (!_isInStringLiteral && previous == '/')
                {
                    _isInComment = false;

                    i = _line.size(); // remaining is a comment line, we can stop here
                }
                break;
            }
        }

        previous = current;
    }

    addCurrentLine(isCommentLineOnly ? Source::Line::Type::Comment : Source::Line::Type::Cpp);

    return true;
}

void SourceReader::setPreviousCommentAndEmptyLinesAsCpps()
{
    if (_source.size() > 1)
    {
        for (Source::Line& line : ranges::views::reverse(_source) | ranges::views::drop(1))
        {
            const Source::Line::Type lineType = line.getType();

            if (lineType != Source::Line::Type::Comment && lineType != Source::Line::Type::Empty)
            {
                break;
            }

            line.setType(Source::Line::Type::Cpps);
        }
    }
}

void SourceReader::addCurrentLine(Source::Line::Type type)
{
    _source.add(std::string(_rawLine), type);
}

} // namespace cpps
