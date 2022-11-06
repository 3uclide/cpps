#include "cpps/lexeme.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

namespace CPPS {

template<typename T>
void check(Lexeme::Type expectedType, std::size_t typeCount, auto typeGetter)
{
    for (std::size_t i = 0; i < typeCount; ++i)
    {
        const T value = static_cast<T>(i);
        const Lexeme lexeme{value};

        CHECK(lexeme.getType() == expectedType);
        CHECK((lexeme.*typeGetter)() == value);
        CHECK(lexeme == value);
    }
}

TEST_CASE("Lexeme", "[Lexeme]")
{
    check<Lexeme::Basic>(Lexeme::Type::Basic, Lexeme::BasicCount, &Lexeme::getBasic);
    check<BooleanLiteral>(Lexeme::Type::BooleanLiteral, BooleanLiteralCount, &Lexeme::getBooleanLiteral);
    check<FunctionModifier>(Lexeme::Type::FunctionModifier, FunctionModifierCount, &Lexeme::getFunctionModifier);
    check<Keyword>(Lexeme::Type::Keyword, KeywordCount, &Lexeme::getKeyword);
    check<ParameterModifier>(Lexeme::Type::ParameterModifier, ParameterModifierCount, &Lexeme::getParameterModifier);
    check<PointerLiteral>(Lexeme::Type::PointerLiteral, PointerLiteralCount, &Lexeme::getPointerLiteral);
}

TEST_CASE("Lexeme format", "[Lexeme]")
{
    CHECK(fmt::format("{}", Lexeme{Lexeme::Ampersand}) == "0");
    CHECK(fmt::format("{}", Lexeme{BooleanLiteral::True}) == toStringView(BooleanLiteral::True));
    CHECK(fmt::format("{}", Lexeme{FunctionModifier::Final}) == toStringView(FunctionModifier::Final));
    CHECK(fmt::format("{}", Lexeme{Keyword::Import}) == toStringView(Keyword::Import));
    CHECK(fmt::format("{}", Lexeme{ParameterModifier::Copy}) == toStringView(ParameterModifier::Copy));
    CHECK(fmt::format("{}", Lexeme{PointerLiteral::NullPtr}) == toStringView(PointerLiteral::NullPtr));
}

TEST_CASE("Lexeme operator<< std::ostream", "[Lexeme]")
{
    std::stringstream stream;

    stream << Lexeme{Lexeme::Ampersand} << "\n";
    stream << Lexeme{BooleanLiteral::True} << "\n";
    stream << Lexeme{FunctionModifier::Final} << "\n";
    stream << Lexeme{Keyword::Import} << "\n";
    stream << Lexeme{ParameterModifier::Copy} << "\n";
    stream << Lexeme{PointerLiteral::NullPtr} << "\n";

    // clang-format off
    std::string expectedString = "0\n"; // Lexeme::Ampersand
    expectedString += toStringView(BooleanLiteral::True); expectedString += "\n";
    expectedString += toStringView(FunctionModifier::Final); expectedString += "\n";
    expectedString += toStringView(Keyword::Import); expectedString += "\n";
    expectedString += toStringView(ParameterModifier::Copy); expectedString += "\n";
    expectedString += toStringView(PointerLiteral::NullPtr); expectedString += "\n";
    // clang-format on

    CHECK(stream.str() == expectedString);
}

} // namespace CPPS
