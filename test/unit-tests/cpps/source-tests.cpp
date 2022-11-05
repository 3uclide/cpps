#include <catch2/catch_test_macros.hpp>

#include "cpps/source.hpp"

namespace cpps {

TEST_CASE("Source default ctor", "[Source]")
{
    const Source source;

    CHECK_FALSE(source.hasCpp());
    CHECK_FALSE(source.hasCpps());
    CHECK(source.empty());
}

TEST_CASE("Source add", "[Source]")
{
    Source source;

    SECTION("will add line text and type")
    {
        static constexpr std::string_view CommentLine = "comment";
        static constexpr std::string_view EmptyLine = "";
        static constexpr std::string_view ImportLine = "import";

        source.add(std::string(CommentLine), Source::Line::Type::Comment);
        source.add(std::string(EmptyLine), Source::Line::Type::Empty);
        source.add(std::string(ImportLine), Source::Line::Type::Import);

        REQUIRE(source.size() == 3);

        CHECK(source[0].getType() == Source::Line::Type::Comment);
        CHECK(source[0].getText() == CommentLine);
        CHECK(source[0].empty() == CommentLine.empty());
        CHECK(source[0].size() == CommentLine.size());

        CHECK(source[1].getType() == Source::Line::Type::Empty);
        CHECK(source[1].getText() == EmptyLine);
        CHECK(source[1].empty() == EmptyLine.empty());
        CHECK(source[1].size() == EmptyLine.size());

        CHECK(source[2].getType() == Source::Line::Type::Import);
        CHECK(source[2].getText() == ImportLine);
        CHECK(source[2].empty() == ImportLine.empty());
        CHECK(source[2].size() == ImportLine.size());
    }

    SECTION("of type cpp will set source has cpp")
    {
        CHECK_FALSE(source.hasCpp());

        source.add("line", Source::Line::Type::Cpp);

        CHECK(source.hasCpp());
    }

    SECTION("of type preprocessor will set source has cpp")
    {
        CHECK_FALSE(source.hasCpp());

        source.add("line", Source::Line::Type::Preprocessor);

        CHECK(source.hasCpp());
    }

    SECTION("of type cpps will set source has cpps")
    {
        CHECK_FALSE(source.hasCpps());

        source.add("line", Source::Line::Type::Cpps);

        CHECK(source.hasCpps());
    }
}

TEST_CASE("Source cppsify", "[Source]")
{
    Source source;

    SECTION("do nothing if singe line")
    {
        source.add("cpps", Source::Line::Type::Cpps);

        source.cppsify();

        REQUIRE(source.size() == 1);
        REQUIRE(source[0].getType() == Source::Line::Type::Cpps);
    }

    SECTION("set previous adjacent comment/empty line type to cpps")
    {
        SECTION("stop when line type is cpp")
        {
            source.add("cpp", Source::Line::Type::Cpp);
            source.add("comment", Source::Line::Type::Comment);
            source.add("", Source::Line::Type::Empty);
            source.add("cpps", Source::Line::Type::Cpps);

            source.cppsify();

            REQUIRE(source.size() == 4);
            REQUIRE(source[0].getType() == Source::Line::Type::Cpp);
            REQUIRE(source[1].getType() == Source::Line::Type::Cpps);
            REQUIRE(source[2].getType() == Source::Line::Type::Cpps);
            REQUIRE(source[3].getType() == Source::Line::Type::Cpps);
        }

        SECTION("stop when line type is import")
        {
            source.add("import", Source::Line::Type::Import);
            source.add("comment", Source::Line::Type::Comment);
            source.add("", Source::Line::Type::Empty);
            source.add("cpps", Source::Line::Type::Cpps);

            source.cppsify();

            REQUIRE(source.size() == 4);
            REQUIRE(source[0].getType() == Source::Line::Type::Import);
            REQUIRE(source[1].getType() == Source::Line::Type::Cpps);
            REQUIRE(source[2].getType() == Source::Line::Type::Cpps);
            REQUIRE(source[3].getType() == Source::Line::Type::Cpps);
        }

        SECTION("stop when line type is preprocessor")
        {
            source.add("preprocessor", Source::Line::Type::Preprocessor);
            source.add("comment", Source::Line::Type::Comment);
            source.add("", Source::Line::Type::Empty);
            source.add("cpps", Source::Line::Type::Cpps);

            source.cppsify();

            REQUIRE(source.size() == 4);
            REQUIRE(source[0].getType() == Source::Line::Type::Preprocessor);
            REQUIRE(source[1].getType() == Source::Line::Type::Cpps);
            REQUIRE(source[2].getType() == Source::Line::Type::Cpps);
            REQUIRE(source[3].getType() == Source::Line::Type::Cpps);
        }
    }
}

} // namespace cpps
