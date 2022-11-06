#include "cpps/lexeme.hpp"

#include <catch2/catch_test_macros.hpp>

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

} // namespace CPPS
