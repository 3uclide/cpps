#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include "cpps/lexeme.hpp"

namespace CPPS {

template<typename T>
void check(std::size_t typeCount, auto typeGetter)
{
    for (std::size_t i = 0; i < typeCount; ++i)
    {
        const T value = static_cast<T>(i);
        const Lexeme lexeme{value};

        CHECK(lexeme.is<T>());
        CHECK((lexeme.*typeGetter)() == value);
        CHECK(lexeme == value);
    }
}

TEST_CASE("Lexeme", "[Lexeme]")
{
    check<BooleanLiteral>(BooleanLiteralCount, &Lexeme::get<BooleanLiteral>);
    check<FunctionModifier>(FunctionModifierCount, &Lexeme::get<FunctionModifier>);
    check<Keyword>(KeywordCount, &Lexeme::get<Keyword>);
    check<ParameterModifier>(ParameterModifierCount, &Lexeme::get<ParameterModifier>);
    check<PointerLiteral>(PointerLiteralCount, &Lexeme::get<PointerLiteral>);
    check<Punctuator>(PunctuatorCount, &Lexeme::get<Punctuator>);
}

TEST_CASE("Lexeme format", "[Lexeme]")
{
    CHECK(fmt::format("{}", Lexeme{}) == "invalid");
    CHECK(fmt::format("{}", Lexeme{BinaryLiteral{}}) == "BinaryLiteral");
    CHECK(fmt::format("{}", Lexeme{BooleanLiteral::True}) == toStringView(BooleanLiteral::True));
    CHECK(fmt::format("{}", Lexeme{FunctionModifier::Final}) == toStringView(FunctionModifier::Final));
    CHECK(fmt::format("{}", Lexeme{Keyword::Import}) == toStringView(Keyword::Import));
    CHECK(fmt::format("{}", Lexeme{ParameterModifier::Copy}) == toStringView(ParameterModifier::Copy));
    CHECK(fmt::format("{}", Lexeme{PointerLiteral::NullPtr}) == toStringView(PointerLiteral::NullPtr));
    CHECK(fmt::format("{}", Lexeme{Punctuator::Ampersand}) == toStringView(Punctuator::Ampersand));
}

TEST_CASE("Lexeme operator<< std::ostream", "[Lexeme]")
{
    std::stringstream stream;

    stream << Lexeme{} << "\n";
    stream << Lexeme{BinaryLiteral{}} << "\n";
    stream << Lexeme{BooleanLiteral::True} << "\n";
    stream << Lexeme{FunctionModifier::Final} << "\n";
    stream << Lexeme{Keyword::Import} << "\n";
    stream << Lexeme{ParameterModifier::Copy} << "\n";
    stream << Lexeme{PointerLiteral::NullPtr} << "\n";
    stream << Lexeme{Punctuator::Ampersand} << "\n";

    std::string expectedString;
    expectedString += "invalid"; // Lexeme{}
    expectedString += "\n";
    expectedString += "BinaryLiteral"; // Lexeme{BinaryLiteral{}}
    expectedString += "\n";
    expectedString += toStringView(BooleanLiteral::True);
    expectedString += "\n";
    expectedString += toStringView(FunctionModifier::Final);
    expectedString += "\n";
    expectedString += toStringView(Keyword::Import);
    expectedString += "\n";
    expectedString += toStringView(ParameterModifier::Copy);
    expectedString += "\n";
    expectedString += toStringView(PointerLiteral::NullPtr);
    expectedString += "\n";
    expectedString += toStringView(Punctuator::Ampersand);
    expectedString += "\n";

    CHECK(stream.str() == expectedString);
}

} // namespace CPPS
