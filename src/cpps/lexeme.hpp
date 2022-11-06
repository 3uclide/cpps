#pragma once

#include <cpps/grammar/boolean-literal.hpp>
#include <cpps/grammar/function-modifier.hpp>
#include <cpps/grammar/keyword.hpp>
#include <cpps/grammar/parameter-modifier.hpp>
#include <cpps/grammar/pointer-literal.hpp>

#include <cassert>
#include <cstdint>

namespace CPPS {

class Lexeme
{
public:
    enum Basic
    {
        Ampersand,
        AmpersandEqual,
        Arrow,
        Assignment,
        BinaryLiteral,
        Caret,
        CaretEqual,
        CharacterLiteral,
        CloseBrace,
        CloseBracket,
        CloseParenthesis,
        Colon,
        Comma,
        CompareEqual,
        CompareNotEqual,
        DecimalLiteral,
        Dollar,
        Dot,
        DoubleColon,
        Ellipsis,
        FloatingLiteral,
        Greater,
        GreaterEqual,
        HexadecimalLiteral,
        Identifier,
        LeftShift,
        LeftShiftEqual,
        Less,
        LessEqual,
        LogicalAnd,
        LogicalAndEqual,
        LogicalOr,
        LogicalOrEqual,
        Minus,
        MinusEqual,
        MinusMinus,
        Modulo,
        ModuloEqual,
        Multiply,
        MultiplyEqual,
        Negation,
        OpenBrace,
        OpenBracket,
        OpenParenthesis,
        Pipe,
        PipeEqual,
        Plus,
        PlusEqual,
        PlusPlus,
        QuestionMark,
        RightShift,
        RightShiftEqual,
        Semicolon,
        Slash,
        SlashEqual,
        Spaceship,
        Tilde,
        TildeEqual,

        BasicCount
    };

    enum class Type : std::uint8_t
    {
        Basic,
        BooleanLiteral,
        FunctionModifier,
        Keyword,
        ParameterModifier,
        PointerLiteral
    };

public:
    constexpr Lexeme() = default;

    constexpr explicit Lexeme(Basic value);
    constexpr explicit Lexeme(BooleanLiteral value);
    constexpr explicit Lexeme(Keyword value);
    constexpr explicit Lexeme(FunctionModifier value);
    constexpr explicit Lexeme(ParameterModifier value);
    constexpr explicit Lexeme(PointerLiteral value);

    constexpr bool operator==(const Lexeme& other) const = default;
    constexpr bool operator==(Basic value) const;
    constexpr bool operator==(BooleanLiteral value) const;
    constexpr bool operator==(Keyword value) const;
    constexpr bool operator==(FunctionModifier value) const;
    constexpr bool operator==(ParameterModifier value) const;
    constexpr bool operator==(PointerLiteral value) const;

    constexpr Type getType() const;

    constexpr Basic getBasic() const;
    constexpr void setBasic(Basic value);

    constexpr BooleanLiteral getBooleanLiteral() const;
    constexpr void setBooleanLiteral(BooleanLiteral value);

    constexpr Keyword getKeyword() const;
    constexpr void setKeyword(Keyword value);

    constexpr FunctionModifier getFunctionModifier() const;
    constexpr void setFunctionModifier(FunctionModifier value);

    constexpr ParameterModifier getParameterModifier() const;
    constexpr void setParameterModifier(ParameterModifier value);

    constexpr PointerLiteral getPointerLiteral() const;
    constexpr void setPointerLiteral(PointerLiteral value);

private:
    using Storage = std::uint16_t;

private:
    constexpr Type getStorageType() const;

    template<typename T>
    requires(std::is_enum_v<T>)
    constexpr T getStorageTypeData() const;

    template<typename T>
    requires(std::is_enum_v<T>)
    constexpr void setStorage(Type type, T data);

    static constexpr Storage StorageTypeMask = 0x00FF;
    static constexpr Storage StorageTypeDataMask = 0xFF00;

    static constexpr Storage InvalidStorage = 0xFFFF;

    Storage _storage{InvalidStorage};
};

constexpr Lexeme::Lexeme(Basic value)
{
    setBasic(value);
}

constexpr Lexeme::Lexeme(PointerLiteral value)
{
    setPointerLiteral(value);
}

constexpr Lexeme::Lexeme(BooleanLiteral value)
{
    setBooleanLiteral(value);
}

constexpr Lexeme::Lexeme(Keyword value)
{
    setKeyword(value);
}

constexpr Lexeme::Lexeme(FunctionModifier value)
{
    setFunctionModifier(value);
}

constexpr Lexeme::Lexeme(ParameterModifier value)
{
    setParameterModifier(value);
}

constexpr bool Lexeme::operator==(Basic value) const
{
    return operator==(Lexeme{value});
}

constexpr bool Lexeme::operator==(BooleanLiteral value) const
{
    return operator==(Lexeme{value});
}

constexpr bool Lexeme::operator==(Keyword value) const
{
    return operator==(Lexeme{value});
}

constexpr bool Lexeme::operator==(FunctionModifier value) const
{
    return operator==(Lexeme{value});
}

constexpr bool Lexeme::operator==(ParameterModifier value) const
{
    return operator==(Lexeme{value});
}

constexpr bool Lexeme::operator==(PointerLiteral value) const
{
    return operator==(Lexeme{value});
}

constexpr Lexeme::Type Lexeme::getType() const
{
    return getStorageType();
}

constexpr Lexeme::Basic Lexeme::getBasic() const
{
    assert(getType() == Type::Basic);
    return getStorageTypeData<Basic>();
}

constexpr void Lexeme::setBasic(Basic value)
{
    setStorage(Type::Basic, value);
}

constexpr BooleanLiteral Lexeme::getBooleanLiteral() const
{
    assert(getType() == Type::BooleanLiteral);
    return getStorageTypeData<BooleanLiteral>();
}

constexpr void Lexeme::setBooleanLiteral(BooleanLiteral value)
{
    setStorage(Type::BooleanLiteral, value);
}

constexpr Keyword Lexeme::getKeyword() const
{
    assert(getType() == Type::Keyword);
    return getStorageTypeData<Keyword>();
}

constexpr void Lexeme::setKeyword(Keyword value)
{
    setStorage(Type::Keyword, value);
}

constexpr FunctionModifier Lexeme::getFunctionModifier() const
{
    assert(getType() == Type::FunctionModifier);
    return getStorageTypeData<FunctionModifier>();
}

constexpr void Lexeme::setFunctionModifier(FunctionModifier value)
{
    setStorage(Type::FunctionModifier, value);
}

constexpr ParameterModifier Lexeme::getParameterModifier() const
{
    assert(getType() == Type::ParameterModifier);
    return getStorageTypeData<ParameterModifier>();
}

constexpr void Lexeme::setParameterModifier(ParameterModifier value)
{
    setStorage(Type::ParameterModifier, value);
}

constexpr PointerLiteral Lexeme::getPointerLiteral() const
{
    assert(getType() == Type::PointerLiteral);
    return getStorageTypeData<PointerLiteral>();
}

constexpr void Lexeme::setPointerLiteral(PointerLiteral value)
{
    setStorage(Type::PointerLiteral, value);
}

constexpr Lexeme::Type Lexeme::getStorageType() const
{
    return static_cast<Type>(_storage & StorageTypeMask);
}

template<typename T>
requires(std::is_enum_v<T>)
constexpr T Lexeme::getStorageTypeData() const
{
    return static_cast<T>((_storage & StorageTypeDataMask) >> 8u);
}

template<typename T>
requires(std::is_enum_v<T>)
constexpr void Lexeme::setStorage(Type type, T data)
{
    _storage = static_cast<Storage>(type) | static_cast<Storage>(static_cast<Storage>(data) << 8u);
}

} // namespace CPPS
