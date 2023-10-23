cmake_minimum_required(VERSION 3.20)
project(NEPS2)


# Enforce MSVC
if (NOT MSVC)
	message(FATAL_ERROR "Sowwey :c Only MSVC is supported currently ;w;")
endif()


###########################################################################################
# Required packages
find_package(Vulkan REQUIRED)

# Find source files recursively
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCE_FILES
	${SOURCE_DIR}/*.cxx
	${SOURCE_DIR}/*.cpp
	${SOURCE_DIR}/*.c
)

file(GLOB_RECURSE SOURCE_FILES2
	${SOURCE_DIR}/include/*.cxx
	${SOURCE_DIR}/include/*.cpp
	${SOURCE_DIR}/include/*.c
	
	${SOURCE_DIR}/cs2/*.cxx
	${SOURCE_DIR}/neps/*.cxx
)

list(APPEND SOURCE_FILES ${SOURCE_FILES2})

# Create target
add_library(NEPS2_DLL SHARED ${SOURCE_FILES})

# Set the precompiled header file
target_precompile_headers(NEPS2_DLL PRIVATE
	$<$<COMPILE_LANGUAGE:CXX>:${SOURCE_DIR}/pch.hpp>
	$<$<COMPILE_LANGUAGE:C>:stddef.h>
)

# Add used include directories to include search paths
target_include_directories(NEPS2_DLL PRIVATE ${SOURCE_DIR})
target_include_directories(NEPS2_DLL PRIVATE ${SOURCE_DIR}/include)

# Add include dirs after find_package beautifully fails to
target_include_directories(NEPS2_DLL PRIVATE ${Vulkan_INCLUDE_DIRS})

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
target_compile_features(NEPS2_DLL PRIVATE cxx_std_23)

# Link static libraries
if(MSVC)
	target_link_libraries(NEPS2_DLL d3d11.lib)
	# find_package apparently fails to do this
	target_link_libraries(NEPS2_DLL ${Vulkan_LIBRARIES})
endif()

# Set the output filename
set_target_properties(NEPS2_DLL PROPERTIES
	OUTPUT_NAME "NEPS2"
	SUFFIX ".dll"
)


###########################################################################################
# Tests
find_package(Python)
enable_testing()
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_test(NAME Sanity COMMAND ${Python_EXECUTABLE} ${TESTS_DIR}/sanity.py)
