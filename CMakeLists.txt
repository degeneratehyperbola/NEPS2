cmake_minimum_required(VERSION 3.20)
project(NEPS2)

# Check for Vulkan SDK
if(NOT DEFINED ENV{VK_SDK_PATH})
	message(FATAL_ERROR "Vulkan SDK not found. I ain't compiling without it, sowwy :3")
endif()

# Enforce MSVC
if (NOT MSVC)
	message(FATAL_ERROR "Sowwey :c Only MSVC is supported currently ;w;")
endif()

# Set the precompiled header file
set(PCH_FILE "PCH.h")

# Add root directory and used include directories as include search paths
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories($ENV{VK_SDK_PATH}/Include)

# Disable compiler optimizations and enable debug info
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	if(MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /Zi")
	endif()

	add_definitions(-D_DEBUG)
# Enable compiler optimizations and loop parallelization
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	if(MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /2d /Qpar")
	endif()
endif()

# Set default function calling convention to `__vectorcall` to speed up calls
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gv")
endif()

# Disable exceptions
# We need to replace the cl.exe argument because it appears to be predefined
if(MSVC)
	string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

add_definitions(-D_HAS_EXCEPTIONS=0)

# Find source files recursively
file(GLOB SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

file(GLOB_RECURSE SOURCE_FILES2
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.c

	${CMAKE_CURRENT_SOURCE_DIR}/CS2/*.cxx
)

list(APPEND SOURCE_FILES ${SOURCE_FILES2})

# DLL debug target
add_library(MAIN_DLL SHARED ${SOURCE_FILES})

# Set the C++ standard
set_target_properties(MAIN_DLL PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED TRUE
)

# Add the static libraries
if(MSVC)
	target_link_libraries(MAIN_DLL d3d11.lib)
	target_link_libraries(MAIN_DLL $ENV{VK_SDK_PATH}/Lib/vulkan-1.lib)
endif()

# Set the output filename
set_target_properties(MAIN_DLL PROPERTIES
	OUTPUT_NAME "NEPS2.debug"
	SUFFIX ".dll"
)
