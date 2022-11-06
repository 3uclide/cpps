#include "cpps/source-reader.hpp"

#include "cpps/check-diagnosis.hpp"
#include "cpps/diagnosis.hpp"

#include <catch2/catch_test_macros.hpp>

namespace cpps {

TEST_CASE("SourceReader", "[SourceReader]")
{
    struct Line
    {
        std::string text;
        Source::Line::Type type;
        bool addNewLine{true};
    };

    auto check = [](std::string_view info, std::initializer_list<Line> initLines) {
        INFO(info);

        std::vector<Line> lines{initLines};

        std::stringstream stream;

        for (const Line& line : lines)
        {
            stream << line.text;

            if (line.addNewLine)
            {
                stream << '\n';
            }
        }

        Diagnosis diagnosis;
        SourceReader sourceReader{diagnosis, stream};

        const std::optional optSource = sourceReader.read();

        checkNoErrorOrWarning(diagnosis);

        REQUIRE(optSource.has_value());

        const Source& source = *optSource;

        REQUIRE(source.size() == lines.size());

        for (std::size_t i = 0; i < lines.size(); ++i)
        {
            INFO(fmt::format("[{}] #{}: {}", static_cast<int>(lines[i].type), i + 1, lines[i].text));

            CHECK(source[static_cast<SourceLine>(i)].getType() == lines[i].type);
            CHECK(source[static_cast<SourceLine>(i)].getText() == lines[i].text);
        }
    };

    SECTION("Line Type Cpp")
    {
        check("cpp line",
              {{"int main() { return 0; }", Source::Line::Type::Cpp}});

        check("cpp line start with comment /**/",
              {{"/* a comment */ int main() { return 0; }", Source::Line::Type::Cpp}});

        check("cpp line end with comment /**/",
              {{"int main() { return 0; } /* a comment */", Source::Line::Type::Cpp}});

        check("cpp lines",
              {{"int main()", Source::Line::Type::Cpp},
               {"{", Source::Line::Type::Cpp},
               {"    return 0;", Source::Line::Type::Cpp},
               {"}", Source::Line::Type::Cpp}});

        check("cpp lines with comments",
              {{"int main()", Source::Line::Type::Cpp},
               {"{ // some comment", Source::Line::Type::Cpp},
               {"    return 0;", Source::Line::Type::Cpp},
               {"} /* another comment */", Source::Line::Type::Cpp}});

        check("cpp line identifier only",
              {{"identifier  ", Source::Line::Type::Cpp, false}});
    }

    SECTION("Line Type Cpps")
    {
        check("cpps line {}",
              {{"identifier: () = {}", Source::Line::Type::Cpps, false}});

        check("cpps lines {}",
              {{"identifier: () = {", Source::Line::Type::Cpps},
               {"", Source::Line::Type::Cpps},
               {"}", Source::Line::Type::Cpps}});

        check("cpps lines with comment //",
              {{"identifier: () = {", Source::Line::Type::Cpps},
               {"// comment", Source::Line::Type::Cpps},
               {"}", Source::Line::Type::Cpps}});

        check("cpps lines with comment /*/",
              {{"identifier: () = {", Source::Line::Type::Cpps},
               {"/* comment */", Source::Line::Type::Cpps},
               {"}", Source::Line::Type::Cpps}});

        check("cpps line ;",
              {{"identifier: () = func();", Source::Line::Type::Cpps, false}});

        check("cpps lines ;",
              {{"identifier: () =", Source::Line::Type::Cpps},
               {"    func();", Source::Line::Type::Cpps}});

        check("cpps override previous comment/empty lines",
              {{"// some comment", Source::Line::Type::Cpps},
               {"", Source::Line::Type::Cpps},
               {"identifier: do_something();", Source::Line::Type::Cpps}});

        check("cpps override previous comment/empty lines stop on cpp line",
              {{"int i = 0;", Source::Line::Type::Cpp},
               {"// some comment", Source::Line::Type::Cpps},
               {"", Source::Line::Type::Cpps},
               {"identifier: do_something();", Source::Line::Type::Cpps}});

        check("cpps override previous comment/empty lines stop on import line",
              {{"import std.core;", Source::Line::Type::Import},
               {"// some comment", Source::Line::Type::Cpps},
               {"", Source::Line::Type::Cpps},
               {"identifier: do_something();", Source::Line::Type::Cpps}});

        check("cpps override previous comment/empty lines stop on preprocessor line",
              {{"#define cpps", Source::Line::Type::Preprocessor},
               {"// some comment", Source::Line::Type::Cpps},
               {"", Source::Line::Type::Cpps},
               {"identifier: do_something();", Source::Line::Type::Cpps}});
    }

    SECTION("Line Type Comment")
    {
        check("comment line //",
              {{"// some comment", Source::Line::Type::Comment}});

        check("comment line /**/",
              {{"/* some comment */", Source::Line::Type::Comment}});

        check("comment lines /**/",
              {{"/* some", Source::Line::Type::Comment},
               {"comment */", Source::Line::Type::Comment}});
    }

    SECTION("Line Type Empty")
    {
        check("empty",
              {{"", Source::Line::Type::Empty},
               {"", Source::Line::Type::Empty}});
    }

    SECTION("Line Type Import")
    {
        check("import",
              {{"import <iostream>", Source::Line::Type::Import},
               {"import std.core;", Source::Line::Type::Import}});
    }

    SECTION("Line Type Preprocessor")
    {
        check("preprocessor line",
              {{"#define PREPROCESSOR", Source::Line::Type::Preprocessor, false}});

        check("preprocessor lines",
              {{"#define PREPROCESSOR \\", Source::Line::Type::Preprocessor},
               {"    doSomething();", Source::Line::Type::Preprocessor}});
    }
}

TEST_CASE("SourceReader diagnosis", "[Diagnosis], [SourceReader]")
{
    auto check = [](std::string_view info, const std::vector<std::string>& lines, const std::vector<Diagnosis::Entry>& expectedErrors) {
        INFO(info);

        std::stringstream stream;

        for (const std::string& line : lines)
        {
            stream << line << "\n";
        }

        Diagnosis diagnosis;
        SourceReader sourceReader{diagnosis, stream};

        const std::optional optSource = sourceReader.read();

        checkNoWarning(diagnosis);

        const std::span<const Diagnosis::Entry> errors = diagnosis.getErrors();

        REQUIRE(errors.size() == expectedErrors.size());

        for (std::size_t i = 0; i < expectedErrors.size(); ++i)
        {
            CHECK(errors[i].message == expectedErrors[i].message);
            CHECK(errors[i].fixMessage == expectedErrors[i].fixMessage);
            CHECK(errors[i].location == expectedErrors[i].location);
        }
    };

    SECTION("Line Type Cpp")
    {
        check("unexpected closing brace",
              {"}"},
              {{SourceReader::DiagnosisMessage::unexpectedCloseBrace(), std::nullopt, SourceLocation{0, 0}}});
    }

    SECTION("Line Type Cpps")
    {
        check("unexpected closing brace",
              {"identifier: () = }"},
              {{SourceReader::DiagnosisMessage::unexpectedCloseBrace(), std::nullopt, SourceLocation{0, 17}},
               {SourceReader::DiagnosisMessage::unreadableLine(), std::nullopt, SourceLocation{0, InvalidSourceColumn}}});

        check("unexpected character after statement end",
              {"identifier: do_something();a"},
              {{SourceReader::DiagnosisMessage::unexpectedCharAfterCppsDefinition('a'), std::nullopt, SourceLocation{0, 27}}});
    }

    SECTION("Line Type Any")
    {
        std::string lineTooLong;
        lineTooLong.resize(SourceReader::BufferCapacity + 1, 'a');
        lineTooLong[SourceReader::BufferCapacity] = '\0';
        check("line too long",
              {lineTooLong},
              {{SourceReader::DiagnosisMessage::lineTooLong(), std::nullopt, SourceLocation{0, InvalidSourceColumn}}});
    }
}

} // namespace cpps
