# test dependencies
set(tests_DEPENDENCIES_CONFIGURED Catch2 fmt)

foreach(DEPENDENCY ${tests_DEPENDENCIES_CONFIGURED})
  find_package(${DEPENDENCY} CONFIG REQUIRED)
endforeach()

# include(CTest)
include(Catch)

set(CPPS_UNIT_TESTS_INCLUDES
    check-diagnosis.hpp
)

set(CPPS_UNIT_TESTS_SOURCES
    check-diagnosis.cpp
    diagnosis-tests.cpp
    lexeme-tests.cpp
    source-location-tests.cpp
    source-reader-tests.cpp
    source-tests.cpp
    utility-tests.cpp
)

add_executable(cpps-unit-tests ${CPPS_UNIT_TESTS_INCLUDES} ${CPPS_UNIT_TESTS_SOURCES})

target_link_libraries(
    cpps-unit-tests
    PUBLIC project_options project_warnings
    PRIVATE cpps Catch2::Catch2WithMain fmt::fmt)

target_include_directories(cpps-unit-tests PRIVATE "../")

target_disable_static_analysis(cpps-unit-tests)

catch_discover_tests(cpps-unit-tests)
