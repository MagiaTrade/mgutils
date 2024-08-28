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
    src/Json.cpp include/mgutils/Json.h
    src/Json.cpp src/Json.cpp
    src/json/JsonDocument.cpp
    include/mgutils/json/JsonDocument.h
    src/json/JsonValue.cpp
    include/mgutils/json/JsonValue.h
    include/mgutils/CsvLoader.h
    include/mgutils/Files.h
    include/mgutils/Exceptions.h)

add_library(${PROJECT_NAME}  STATIC
    ${MG_UTILS_FILES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

# Include FetchContent module
include(FetchContent)

# Set the base directory for external dependencies
set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

# Ensure the external directory exists
file(MAKE_DIRECTORY ${EXTERNAL_DIR})

# Download and add spdlog to the external directory
message(STATUS "Fetching spdlog...")
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG 5ebfc927306fd7ce551fa22244be801cf2b9fdd9
)
FetchContent_MakeAvailable(spdlog)
message(STATUS "spdlog fetched and made available.")

# Download and add catch2 to the external directory
message(STATUS "Fetching catch2...")
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG ee1450f268dfd5c13aa8670ba97e93cabaf2e15d
    SOURCE_DIR ${EXTERNAL_DIR}/catch2
)
FetchContent_MakeAvailable(catch2)
message(STATUS "catch2 fetched and made available.")

# Download and add oneTBB to the external directory
message(STATUS "Fetching oneTBB...")
FetchContent_Declare(
    onetbb
    GIT_REPOSITORY https://github.com/oneapi-src/oneTBB.git
    GIT_TAG aae7fa62d1ffecd0af05bbd328899845c33be327
    SOURCE_DIR ${EXTERNAL_DIR}/onetbb
)
set(BUILD_SHARED_LIBS ON)
FetchContent_MakeAvailable(onetbb)
set(BUILD_SHARED_LIBS OFF)
message(STATUS "oneTBB fetched and made available.")

# Download and add rapidjson to the external directory
message(STATUS "Fetching rapidjson...")
FetchContent_Declare(
    rapidjson
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    GIT_TAG 7c73dd7de7c4f14379b781418c6e947ad464c818
    SOURCE_DIR ${EXTERNAL_DIR}/rapidjson
)
set(RAPIDJSON_BUILD_DOC OFF)
set(RAPIDJSON_BUILD_EXAMPLES OFF)
set(RAPIDJSON_BUILD_TESTS OFF)
set(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT OFF)

FetchContent_MakeAvailable(rapidjson)
message(STATUS "rapidjson fetched and made available.")

# Download and add rapidcsv to the external directory
message(STATUS "Fetching rapidcsv...")

FetchContent_Declare(
    rapidcsv
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
    GIT_TAG f438d3774848e3c5e281fb15e12f97c0872cf6a8
    SOURCE_DIR ${EXTERNAL_DIR}/rapidcsv
)

FetchContent_MakeAvailable(rapidcsv)
message(STATUS "rapidcsv fetched and made available.")

## BOOST BEGIN -------------
find_package(Boost REQUIRED)

if (${Boost_FOUND})
    target_include_directories( ${PROJECT_NAME} PUBLIC
        ${Boost_INCLUDE_DIRS}
    )
    message(STATUS "Boost version: ${Boost_VERSION_STRING}")

else()
    message(FATAL_ERROR "Boost not found please install it! Instructions on readme.")
endif()
## BOOST END -------------


target_include_directories(${PROJECT_NAME} PUBLIC
    include
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include/mgutils
    include/mgutils/json
    external/rapidcsv/src
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
