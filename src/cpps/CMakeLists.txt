find_package(fmt CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

set(CPPS_INCLUDES
    source-location.hpp
    source-reader.hpp
)

set(CPPS_SOURCES
    source-reader.cpp
)

add_library(cpps STATIC ${CPPS_INCLUDES} ${CPPS_SOURCES})

target_link_libraries(
    cpps
    PUBLIC project_options project_warnings
    PRIVATE fmt::fmt range-v3::range-v3)

target_include_directories(cpps PUBLIC "../")
