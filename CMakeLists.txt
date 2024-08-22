cmake_minimum_required(VERSION 3.12)
project(mgutils)

set(CMAKE_CXX_STANDARD 17)

set(MGUTILS_BUILD_TESTS ON)
set(MGUTILS_BUILD_EXAMPLES ON)

set(MG_UTILS_FILES
    include/mgutils/Logger.h
    src/Logger.cpp
    include/mgutils/Utils.h
)

add_library(${PROJECT_NAME}  STATIC
    ${MG_UTILS_FILES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

## External deps BEGIN -----------
set(EXTERNAL_DIR external)

if(NOT EXISTS "${EXTERNAL_DIR}")
    message(STATUS "Directory ${EXTERNAL_DIR} does not exist. Creating and running script...")

    file(MAKE_DIRECTORY  "${EXTERNAL_DIR}")

    execute_process(
        COMMAND bash "${CMAKE_SOURCE_DIR}/install-deps.sh"
        WORKING_DIRECTORY  "${CMAKE_SOURCE_DIR}"
        RESULT_VARIABLE result
    )

    if(result)
        message(FATAL_ERROR "Script fail with code: ${result}")
    endif()
else()
    message(STATUS "Directory ${EXTERNAL_DIR} already exists. Skipping script execution.")
endif()

add_subdirectory(external/spdlog)
## External deps END -----------

## BOOST BEGIN -------------
find_package(Boost REQUIRED)

if (${Boost_FOUND})
    target_include_directories( ${PROJECT_NAME} PUBLIC
        ${Boost_INCLUDE_DIRS}
    )
else()
    message(FATAL_ERROR "Boost not found please install it!")
endif()
## BOOST END -------------


target_include_directories(${PROJECT_NAME} PUBLIC
    include
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include/mgutils
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    spdlog::spdlog_header_only
)

if (${MGUTILS_BUILD_TESTS})
    add_subdirectory(tests)
endif()

if (${MGUTILS_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()
