find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

set(CPPS_INCLUDES
    cst/basic-expression.hpp
    cst/binary-expression.hpp
    cst/compound-statement.hpp
    cst/declaration.hpp
    cst/declaration-list.hpp
    cst/expression.hpp
    cst/expression-list.hpp
    cst/expression-statement.hpp
    cst/function-signature.hpp
    cst/identifier.hpp
    cst/identifier-expression.hpp
    cst/iteration-statement.hpp
    cst/node.hpp
    cst/node-list.hpp
    cst/node-variant.hpp
    cst/parameter-declaration.hpp
    cst/parameter-declaration-list.hpp
    cst/parser.hpp
    cst/postfix-expression.hpp
    cst/prefix-expression.hpp
    cst/primary-expression.hpp
    cst/qualified-identifier.hpp
    cst/return-statement.hpp
    cst/selection-statement.hpp
    cst/statement.hpp
    cst/statement-list.hpp
    cst/translation-unit.hpp
    cst/unqualified-identifier.hpp
    grammar/binary-literal.hpp
    grammar/boolean-literal.hpp
    grammar/character-literal.hpp
    grammar/decimal-literal.hpp
    grammar/floating-literal.hpp
    grammar/function-modifier.hpp
    grammar/hexadecimal-literal.hpp
    grammar/identifier.hpp
    grammar/keyword.hpp
    grammar/parameter-modifier.hpp
    grammar/pointer-literal.hpp
    grammar/punctuator.hpp
    grammar/string-literal.hpp
    utility/bump-pointer-allocator.hpp
    utility/strings.hpp
    utility/type-list.hpp
    comment.hpp
    cst.hpp
    diagnosis.hpp
    generator.hpp
    lexeme.hpp
    lexer.hpp
    source.hpp
    source-location.hpp
    source-reader.hpp
    token.hpp
    tokens.hpp
)

set(CPPS_SOURCES
    cst/basic-expression.cpp
    cst/declaration.cpp
    cst/declaration-list.cpp
    cst/expression.cpp
    cst/expression-list.cpp
    cst/expression-statement.cpp
    cst/function-signature.cpp
    cst/identifier.cpp
    cst/identifier-expression.cpp
    cst/node.hpp
    cst/node-list.hpp
    cst/node-variant.hpp
    cst/parameter-declaration.cpp
    cst/parameter-declaration-list.cpp
    cst/parser.cpp
    cst/postfix-expression.cpp
    cst/prefix-expression.cpp
    cst/primary-expression.cpp
    cst/return-statement.cpp
    cst/statement.cpp
    cst/statement-list.cpp
    grammar/boolean-literal.cpp
    grammar/function-modifier.cpp
    grammar/keyword.cpp
    grammar/parameter-modifier.cpp
    grammar/pointer-literal.cpp
    grammar/punctuator.cpp
    generator.cpp
    lexer.cpp
    source-reader.cpp

    # TODO cleaner integration
    cpps.natvis
)

add_library(cpps STATIC ${CPPS_INCLUDES} ${CPPS_SOURCES})

target_link_libraries(
    cpps
    PUBLIC project_options project_warnings fmt::fmt range-v3::range-v3)

target_include_directories(cpps PUBLIC "../")
