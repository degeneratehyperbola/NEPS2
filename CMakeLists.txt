cmake_minimum_required(VERSION 3.20)
project(NEPS2)

# Required packages
find_package(Vulkan REQUIRED)
find_package(Python 3.11 REQUIRED COMPONENTS Interpreter Development)

# Enforce MSVC
if (NOT MSVC)
	message(FATAL_ERROR "Sowwey :c Only MSVC is supported currently ;w;")
endif()

# Set the precompiled header file
set(PCH_FILE "PCH.hpp")

# Add root directory and used include directories as include search paths
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add include dirs after find_package beautifully fails to
include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${Python_INCLUDE_DIRS})

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
	${CMAKE_CURRENT_SOURCE_DIR}/NEPS/*.cxx
)

list(APPEND SOURCE_FILES ${SOURCE_FILES2})

# DLL target
add_library(NEPS2_DLL SHARED ${SOURCE_FILES})

# Pre-processor macro definitions
target_compile_definitions(NEPS2_DLL PRIVATE
	$<$<CONFIG:Debug>:_DEBUG>
	$<$<CONFIG:Release>:NDEBUG>
	NOMINMAX
)

# Set compiler options
if(MSVC)
	target_compile_options(NEPS2_DLL PRIVATE
		$<$<CONFIG:Debug>:/Od /Zi>
		$<$<CONFIG:Release>:/2d /Qpar>
		# /Gv
	)
endif()

# Set the C++ standard
set_target_properties(NEPS2_DLL PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED TRUE
)

# Link static libraries
if(MSVC)
	target_link_libraries(NEPS2_DLL d3d11.lib)
	# find_package apparently fails to do this
	target_link_libraries(NEPS2_DLL ${Vulkan_LIBRARIES})
	target_link_libraries(NEPS2_DLL ${Python_LIBRARIES})
endif()

# Set the output filename
set_target_properties(NEPS2_DLL PROPERTIES
	OUTPUT_NAME "NEPS2"
	SUFFIX ".dll"
)
