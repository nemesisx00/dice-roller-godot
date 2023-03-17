cmake_minimum_required(VERSION 3.25)
project(nodes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
	GodotCpp
	GIT_REPOSITORY https://github.com/godotengine/godot-cpp
	GIT_TAG godot-4.0-stable
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(GodotCpp)

file(
	GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

if(CMAKE_HOST_WIN32)
	add_compile_definitions(TYPED_METHOD_BIND)
	add_compile_definitions(NOMINMAX)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} PUBLIC godot::cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX src FILES ${SOURCES})
