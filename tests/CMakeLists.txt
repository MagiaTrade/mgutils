cmake_minimum_required(VERSION 3.12)
project(mgutils_tests VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

set(ENABLE_TESTS OFF)
add_executable(${PROJECT_NAME}
    main.cpp
    cases/logger_tests.cpp
    cases/error_tests.cpp
    cases/jobpool_tests.cpp
    cases/events_tests.cpp
    cases/json_tests.cpp
    cases/csv_tests.cpp
    cases/files_tests.cpp
    )

set(TEST_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_custom_target(copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${TEST_RESOURCES_DIR} ${CMAKE_CURRENT_BINARY_DIR}/resources
)

add_dependencies(${PROJECT_NAME}  copy_resources)

target_link_libraries(${PROJECT_NAME} PRIVATE mgutils)

target_include_directories(${PROJECT_NAME} PUBLIC
    ./
)