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
)

target_link_libraries(${PROJECT_NAME} PRIVATE mgutils)

target_include_directories(${PROJECT_NAME} PUBLIC
    ./
)