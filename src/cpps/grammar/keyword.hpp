#pragma once

#include <array>
#include <ostream>
#include <string_view>

#include <fmt/format.h>

namespace CPPS {

// TODO Split keywords into categories (e.g. primitive types, visibility, casts, etc..)
// TODO Allow other grammar enum to 'identify' as a Keyword
enum class Keyword
{
    AlignAs,
    AlignOf,
    As,
    Asm,
    Auto,
    Bool,
    Break,
    Case,
    Catch,
    Char,
    Char16_t,
    Char32_t,
    Char8_t,
    Class,
    CoAwait,
    CoReturn,
    CoYield,
    Concept,
    Const,
    ConstCast,
    ConstEval,
    ConstExpr,
    ConstInit,
    Continue,
    DeclType,
    Default,
    Do,
    Double,
    DynamicCast,
    Else,
    Enum,
    Explicit,
    Export,
    Extern,
    Float,
    For,
    Friend,
    Goto,
    If,
    Import,
    Inline,
    Int,
    Is,
    Long,
    Module,
    Mutable,
    Namespace,
    New,
    NoExcept,
    Not,
    Operator,
    Private,
    Protected,
    Public,
    Register,
    ReinterpretCast,
    Requires,
    Return,
    Short,
    Signed,
    SizeOf,
    Static,
    StaticAssert,
    StaticCast,
    Struct,
    Switch,
    Template,
    This,
    ThreadLocal,
    Throw,
    Throws,
    Try,
    TypeDef,
    TypeId,
    TypeName,
    Unsigned,
    Using,
    Void,
    Volatile,
    Wchar_t,
    While
};

std::ostream& operator<<(std::ostream& os, Keyword value);

inline constexpr std::array KeywordStrings = std::to_array<std::string_view>(
    {"alignas",
     "alignof",
     "as",
     "asm",
     "auto",
     "bool",
     "break",
     "case",
     "catch",
     "char",
     "char16_t",
     "char32_t",
     "char8_t",
     "class",
     "co_await",
     "co_return",
     "co_yield",
     "concept",
     "const",
     "const_cast",
     "consteval",
     "constexpr",
     "constinit",
     "continue",
     "decltype",
     "default",
     "do",
     "double",
     "dynamic_cast",
     "else",
     "enum",
     "explicit",
     "export",
     "extern",
     "float",
     "for",
     "friend",
     "goto",
     "if",
     "import",
     "inline",
     "int",
     "is",
     "long",
     "module",
     "mutable",
     "namespace",
     "new",
     "noexcept",
     "not",
     "operator",
     "private",
     "protected",
     "public",
     "register",
     "reinterpret_cast",
     "requires",
     "return",
     "short",
     "signed",
     "sizeof",
     "static",
     "static_assert",
     "static_cast",
     "struct",
     "switch",
     "template",
     "this",
     "thread_local",
     "throw",
     "throws",
     "try",
     "typedef",
     "typeid",
     "typename",
     "unsigned",
     "using",
     "void",
     "volatile",
     "wchar_t",
     "while"});

inline constexpr std::size_t KeywordCount = KeywordStrings.size();

constexpr std::string_view toStringView(Keyword value)
{
    return KeywordStrings[static_cast<std::size_t>(value)];
}

} // namespace CPPS

template<>
struct fmt::formatter<CPPS::Keyword> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(CPPS::Keyword value, FormatContext& ctx) const
    {
        return formatter<std::string_view>::format(toStringView(value), ctx);
    }
};
