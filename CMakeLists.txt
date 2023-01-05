cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project("mlog-compiler" CXX)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/source/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/header/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/header")

target_compile_options(${PROJECT_NAME} PUBLIC
	$<IF:${MSVC},
		/W4 /WX, 
		-Wall -Wextra -pedantic -Werror -Wno-reorder
	>
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
