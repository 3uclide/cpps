#pragma once

#include "cpps/grammar/binary-literal.hpp"
#include "cpps/grammar/boolean-literal.hpp"
#include "cpps/grammar/character-literal.hpp"
#include "cpps/grammar/decimal-literal.hpp"
#include "cpps/grammar/floating-literal.hpp"
#include "cpps/grammar/function-modifier.hpp"
#include "cpps/grammar/hexadecimal-literal.hpp"
#include "cpps/grammar/identifier.hpp"
#include "cpps/grammar/keyword.hpp"
#include "cpps/grammar/parameter-modifier.hpp"
#include "cpps/grammar/pointer-literal.hpp"
#include "cpps/grammar/punctuator.hpp"
#include "cpps/grammar/string-literal.hpp"
#include "cpps/utility/type-list.hpp"

#include <cassert>
#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/format.h>

namespace CPPS {

using LexemeTypes = TypeList<BinaryLiteral,
                             BooleanLiteral,
                             CharacterLiteral,
                             DecimalLiteral,
                             FloatingLiteral,
                             FunctionModifier,
                             HexadecimalLiteral,
                             Identifier,
                             Keyword,
                             ParameterModifier,
                             PointerLiteral,
                             Punctuator,
                             StringLiteral>;

template<typename T>
concept Lexemable = TypeListContainsV<T, LexemeTypes>;

class Lexeme
{
public:
    constexpr Lexeme() = default;
    constexpr bool operator==(const Lexeme& other) const = default;

    template<Lexemable T>
    constexpr explicit Lexeme(T value);

    template<Lexemable T>
    constexpr bool operator==(T value) const;

    template<Lexemable T>
    [[nodiscard]] constexpr bool is() const;

    template<Lexemable T>
    [[nodiscard]] constexpr T get() const;

    template<Lexemable T>
    constexpr void set(T value);

private:
    using Data = std::uint16_t;

    static constexpr Data DataTypeMask = 0x00FF;
    static constexpr Data DataTypeValueMask = 0xFF00;
    static constexpr Data InvalidData = 0xFFFF;

    Data _data{InvalidData};
};

std::ostream& operator<<(std::ostream& os, Lexeme value);

template<Lexemable T>
constexpr Lexeme::Lexeme(T value)
{
    set(value);
}

template<Lexemable T>
constexpr bool Lexeme::operator==(T value) const
{
    return operator==(Lexeme{value});
}

template<Lexemable T>
constexpr bool Lexeme::is() const
{
    return (_data & DataTypeMask) == IndexOfFromTypeListV<T, LexemeTypes>;
}

template<Lexemable T>
constexpr T Lexeme::get() const
{
    assert(is<T>());
    return static_cast<T>((_data & DataTypeValueMask) >> 8u);
}

template<Lexemable T>
constexpr void Lexeme::set(T value)
{
    _data = static_cast<Data>(IndexOfFromTypeListV<T, LexemeTypes>) | static_cast<Data>(static_cast<Data>(value) << 8u);
}

} // namespace CPPS

template<>
struct fmt::formatter<CPPS::Lexeme> : formatter<std::string>
{
    template<typename FormatContext>
    auto format(CPPS::Lexeme lexeme, FormatContext& ctx) const
    {
        if (lexeme.is<CPPS::BinaryLiteral>()) return fmt::format_to(ctx.out(), "BinaryLiteral");
        if (lexeme.is<CPPS::BooleanLiteral>()) return fmt::format_to(ctx.out(), "{}", lexeme.get<CPPS::BooleanLiteral>());
        if (lexeme.is<CPPS::CharacterLiteral>()) return fmt::format_to(ctx.out(), "CharacterLiteral");
        if (lexeme.is<CPPS::DecimalLiteral>()) return fmt::format_to(ctx.out(), "DecimalLiteral");
        if (lexeme.is<CPPS::FloatingLiteral>()) return fmt::format_to(ctx.out(), "FloatingLiteral");
        if (lexeme.is<CPPS::HexadecimalLiteral>()) return fmt::format_to(ctx.out(), "HexadecimalLiteral");
        if (lexeme.is<CPPS::FunctionModifier>()) return fmt::format_to(ctx.out(), "{}", lexeme.get<CPPS::FunctionModifier>());
        if (lexeme.is<CPPS::Identifier>()) return fmt::format_to(ctx.out(), "Identifier");
        if (lexeme.is<CPPS::Keyword>()) return fmt::format_to(ctx.out(), "{}", lexeme.get<CPPS::Keyword>());
        if (lexeme.is<CPPS::ParameterModifier>()) return fmt::format_to(ctx.out(), "{}", lexeme.get<CPPS::ParameterModifier>());
        if (lexeme.is<CPPS::PointerLiteral>()) return fmt::format_to(ctx.out(), "{}", lexeme.get<CPPS::PointerLiteral>());
        if (lexeme.is<CPPS::Punctuator>()) return fmt::format_to(ctx.out(), "{}", lexeme.get<CPPS::Punctuator>());
        if (lexeme.is<CPPS::StringLiteral>()) return fmt::format_to(ctx.out(), "StringLiteral");

        return fmt::format_to(ctx.out(), "invalid");
    }
};

namespace CPPS {

inline std::ostream& operator<<(std::ostream& os, Lexeme value)
{
    return os << fmt::format("{}", value);
}

} // namespace CPPS
