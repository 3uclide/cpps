find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

set(CPPS_INCLUDES
    grammar/boolean-literal.hpp
    grammar/function-modifier.hpp
    grammar/keyword.hpp
    grammar/parameter-modifier.hpp
    grammar/pointer-literal.hpp
    diagnosis.hpp
    lexeme.hpp
    source.hpp
    source-location.hpp
    source-reader.hpp
    utility.hpp
)

set(CPPS_SOURCES
    grammar/boolean-literal.cpp
    grammar/function-modifier.cpp
    grammar/keyword.cpp
    grammar/parameter-modifier.cpp
    grammar/pointer-literal.cpp
    source-reader.cpp
)

add_library(cpps STATIC ${CPPS_INCLUDES} ${CPPS_SOURCES})

target_link_libraries(
    cpps
    PUBLIC project_options project_warnings fmt::fmt range-v3::range-v3)

target_include_directories(cpps PUBLIC "../")
