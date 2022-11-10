find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

set(CPPS_INCLUDES
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
    utility/type-list.hpp
    comment.hpp
    diagnosis.hpp
    lexeme.hpp
    lexer.hpp
    source.hpp
    source-location.hpp
    source-reader.hpp
    token.hpp
    tokens.hpp
    utility.hpp
)

set(CPPS_SOURCES
    grammar/boolean-literal.cpp
    grammar/function-modifier.cpp
    grammar/keyword.cpp
    grammar/parameter-modifier.cpp
    grammar/pointer-literal.cpp
    grammar/punctuator.cpp
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
