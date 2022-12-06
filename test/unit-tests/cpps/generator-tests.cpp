#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>
#include <tuple>

#include "check-diagnosis.hpp"
#include "cpps/cst.hpp"
#include "cpps/cst/parser.hpp"
#include "cpps/diagnosis.hpp"
#include "cpps/generator.hpp"
#include "cpps/lexer.hpp"
#include "cpps/source-reader.hpp"
#include "cpps/source.hpp"
#include "cpps/tokens.hpp"

namespace CPPS {

template<typename T>
concept Stringable = std::is_convertible_v<T, std::string>;

template<Stringable... LinesT>
auto generate(LinesT&&... lines)
{
    Diagnosis diagnosis;

    std::stringstream inStream;
    ((inStream << lines), ...);

    SourceReader sourceReader{diagnosis, inStream};
    std::optional<Source> source = sourceReader.read();
    REQUIRE(source);
    checkNoErrorOrWarning(diagnosis);

    Lexer lexer{diagnosis, *source};
    Tokens tokens = lexer.lex();
    checkNoErrorOrWarning(diagnosis);

    CST::Parser parser{diagnosis, tokens};
    CST::TranslationUnit tu = parser.parse();
    checkNoErrorOrWarning(diagnosis);

    std::stringstream outStream;
    Generator generator{diagnosis, *source, tokens, tu, outStream};
    generator.generate();

    return std::make_tuple(std::move(*source), std::move(diagnosis), std::move(tokens), outStream.str());
}

TEST_CASE("Generator", "[Generator]")
{
    auto [source, diagnosis, tokens, result] = generate("");
}

} // namespace CPPS
