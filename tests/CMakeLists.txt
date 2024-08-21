cmake_minimum_required(VERSION 3.12)
project(mgu_tils_tests VERSION 1.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    main.cpp
    ${BEAST_BOYS_FILES}
    cases/logger_tests.cpp
)
#
#if (${Boost_FOUND})
#    target_include_directories(${PROJECT_NAME} PUBLIC
#            ${Boost_INCLUDE_DIRS}
#            )
#endif()


target_include_directories(${PROJECT_NAME} PUBLIC
        ./
        ../external/catch2/single_include
        ${BEAST_BOYS_FOLDERS}
        )