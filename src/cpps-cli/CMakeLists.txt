find_package(argparse CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

set(CPPS_CLI_INCLUDES )

set(CPPS_CLI_SOURCES
    main.cpp)

add_executable(cpps-cli ${CPPS_CLI_INCLUDES} ${CPPS_CLI_SOURCES})

target_link_libraries(
  cpps-cli
  PUBLIC project_options project_warnings
  PRIVATE cpps argparse::argparse fmt::fmt)
