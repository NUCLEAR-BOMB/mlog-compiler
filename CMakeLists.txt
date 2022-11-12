cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project("mlog-compiler" CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/compiler/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/compiler/*.hpp")

if (MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable("mlog-compiler" ${SOURCE_FILES} ${HEADER_FILES})

