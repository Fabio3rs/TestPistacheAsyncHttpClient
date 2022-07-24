set(projectname testPistacheClient)

cmake_minimum_required(VERSION 3.6.0)
project(${projectname} VERSION 0.1.1)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    message(STATUS "Setting G++ flags")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -std=c++17 -Wno-exit-time-destructors -Wno-global-constructors")
    set(COMPILE_FLAGS_CUSTOM -Wall -Werror -Wextra -O2 -std=c++17 -Wformat-security -Wconversion -Wsign-conversion)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message(STATUS "Setting Clang flags")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -std=c++17 -Wno-c++98-compat -Wno-padded -Wno-date-time -Wno-c++98-compat-pedantic -Wno-exit-time-destructors -Wno-global-constructors")
    set(COMPILE_FLAGS_CUSTOM -Weverything -Werror -Wno-date-time -Wno-disabled-macro-expansion -Wno-global-constructors -std=c++17 -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-exit-time-destructors)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_DEFAULT 17)
set(CXX17 ON)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_C_EXTENSIONS Off)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CPack)

include(CTest)
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

find_package(Poco REQUIRED COMPONENTS Crypto JSON Net NetSSL Redis)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(PkgConfig)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/cppapiframework)

file(
    GLOB SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/web/*.cpp"
)

file(
    GLOB SOURCESEXE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library("${projectname}_lib" ${SOURCES})
add_executable(${projectname} ${SOURCESEXE})

target_compile_options("${projectname}_lib" PRIVATE ${COMPILE_FLAGS_CUSTOM} -fsanitize=address)
target_compile_options(${projectname} PRIVATE ${COMPILE_FLAGS_CUSTOM} -fsanitize=address)
target_precompile_headers("${projectname}_lib" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.hpp")
target_precompile_headers(${projectname} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.hpp")

target_link_libraries("${projectname}_lib" ${Poco_LIBRARIES})

target_link_libraries("${projectname}_lib" mysqlcppconn cppapiframework)
target_link_libraries(${projectname} "${projectname}_lib")
target_link_options(${projectname} PUBLIC -fsanitize=address)

add_subdirectory(${CMAKE_SOURCE_DIR}/tests)

