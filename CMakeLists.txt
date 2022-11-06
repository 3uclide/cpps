cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# project_options v0.26.1
# https://github.com/cpp-best-practices/project_options
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/cpp-best-practices/project_options/archive/refs/tags/v0.26.1.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

# CPPS
project(
    cpps
    VERSION 0.0.1
    DESCRIPTION "C++Square a programming language that transpile to clean C++."
    LANGUAGES CXX C
)

# options
option(CPPS_ENABLE_TESTS "Enable the tests" OFF)
option(CPPS_ENABLE_COVERAGE "Enable code coverage" OFF)

# prepare options
if(CPPS_ENABLE_COVERAGE)
    enable_testing()
    set(CPPS_ENABLE_COVERAGE "ENABLE_COVERAGE")
elseif(CPPS_ENABLE_TESTS)
    enable_testing()

    set(CPPS_ENABLE_CLANG_TIDY "ENABLE_CLANG_TIDY")
    set(CPPS_ENABLE_CPPCHECK "ENABLE_CPPCHECK")
    set(CPPS_ENABLE_VS_ANALYSIS "ENABLE_VS_ANALYSIS")

    if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        set(CPPS_ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
        set(CPPS_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "ENABLE_SANITIZER_UNDEFINED_BEHAVIOR")
    else()
        # or it is MSVC and has run vcvarsall
        string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)
        if(MSVC AND "${index_of_vs_install_dir}" STREQUAL "-1")
            set(CPPS_ENABLE_SANITIZER_ADDRESS "ENABLE_SANITIZER_ADDRESS")
        endif()
    endif()
endif()

# set options
project_options(
    ENABLE_CACHE
    ENABLE_CONAN
    ${CPPS_ENABLE_CPPCHECK}
    ${CPPS_ENABLE_CLANG_TIDY}
    ENABLE_INTERPROCEDURAL_OPTIMIZATION
    ENABLE_NATIVE_OPTIMIZATION
    ${CPPS_ENABLE_COVERAGE}
    ${CPPS_ENABLE_SANITIZER_ADDRESS}
    ${CPPS_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
    # ENABLE_SANITIZER_LEAK
    # ENABLE_SANITIZER_THREAD
    # ENABLE_SANITIZER_MEMORY
    ${CPPS_ENABLE_VS_ANALYSIS}
    WARNINGS_AS_ERRORS
    # ENABLE_BUILD_WITH_TIME_TRACE
    CPPCHECK_OPTIONS
    --enable=style,performance,warning,portability
    --inline-suppr
    # We cannot act on a bug/missing feature of cppcheck
    --suppress=cppcheckError
    --suppress=internalAstError
    # if a file does not have an internalAstError, we get an unmatchedSuppression error
    --suppress=unmatchedSuppression
    --suppress=passedByValue
    --suppress=syntaxError
    --suppress=functionStatic
    --inconclusive
)

# project options/warnings
target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_library(cpps::project_options INTERFACE IMPORTED)
add_library(cpps::project_warnings INTERFACE IMPORTED)

# project directories
add_subdirectory(src)

if(CPPS_ENABLE_TESTS OR CPPS_ENABLE_COVERAGE)
    add_subdirectory(test)
endif()

# project package
package_project(
    TARGETS
    cpps-cli
    project_options
    project_warnings
)

# git sha
set(CPPS_GIT_SHA "Unknown" CACHE STRING "SHA this build was generated from")
string(SUBSTRING "${CPPS_GIT_SHA}" 0 8 CPPS_GIT_SHORT_SHA)

set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${CPPS_GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
