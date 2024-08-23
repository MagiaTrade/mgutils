cmake_minimum_required(VERSION 3.23)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(mgutils)

set(CMAKE_CXX_STANDARD 17)

option(MGUTILS_BUILD_TESTS "Build the tests" ON)
option(MGUTILS_BUILD_EXAMPLES "Build the examples" ON)

set(MG_UTILS_FILES
    include/mgutils/Logger.h
    src/Logger.cpp
    include/mgutils/Utils.h
    include/mgutils/ErrorManager.h
    include/mgutils/JobPool.h
        include/mgutils/json/Json.cpp include/mgutils/json/Json.h
        include/mgutils/json/Json.cpp include/mgutils/json/Json.cpp
        include/mgutils/json/JsonDocument.cpp include/mgutils/json/JsonDocument.h include/mgutils/json/JsonValue.cpp include/mgutils/json/JsonValue.h)

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

# to honor the value before add the library #
set(TBB_TEST OFF)                            #
# ------------------------------------------ #
add_subdirectory(external/oneTBB)


# turn off rapidjson extra builds
set(RAPIDJSON_BUILD_DOC OFF)
set(RAPIDJSON_BUILD_EXAMPLES OFF)
set(RAPIDJSON_BUILD_TESTS OFF)
set(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT OFF)

add_subdirectory(external/rapidjson)

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

target_include_directories(${PROJECT_NAME} PUBLIC
    include/mgutils
    include/mgutils/json
    external/rapidjson/include
    external/catch2/single_include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    spdlog::spdlog_header_only
    TBB::tbb
)

if (${MGUTILS_BUILD_TESTS})
    add_subdirectory(tests)
endif()

if (${MGUTILS_BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()
