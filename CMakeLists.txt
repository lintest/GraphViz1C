cmake_minimum_required(VERSION 3.10)
project(GraphViz1c)
set(TARGET GraphViz1c)

add_library(${TARGET} SHARED
		src/AddInNative.cpp
		src/AddInNative.def
		src/AddInNative.h
		src/AddInNative.rc
		src/GraphViz1C.cpp
		src/GraphViz1C.h
		src/stdafx.h)

target_compile_definitions(${TARGET} PRIVATE UNICODE _UNICODE)
target_include_directories(${TARGET} PRIVATE include)
set (LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set (EXECUTABLE_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH})
set (CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

if (UNIX)
	if (APPLE)
		set(MySuffix1 "Mac")
	else(APPLE)
		set(MySuffix1 "Lin")
	endif(APPLE)
	if (TARGET_PLATFORM_32)
		set(MySuffix2 "32")
	else()
		set(MySuffix2 "64")
	endif()
else(UNIX)
	if (NOT MSVC)
		message(FATAL_ERROR "Must be compiled with MSVC on Windows")
	endif(NOT MSVC)
	set(MyPrefix "lib")
	set(MySuffix1 "Win")
endif(UNIX)

set_target_properties( ${PROJECT_NAME} PROPERTIES
	   OUTPUT_NAME ${MyPrefix}${PROJECT_NAME}${MySuffix1}${MySuffix2} 
	   POSITION_INDEPENDENT_CODE ON 
	   CXX_STANDARD_REQUIRED ON
	   CXX_STANDARD 17
   )

if (UNIX)
	if (TARGET_PLATFORM_32)
		set(CMAKE_LIBRARY_PATH "/usr/lib/i386-linux-gnu" CACHE PATH "<desc>")
		SET(ARCH_FLAG "-m32")
	else()
		SET(ARCH_FLAG "-m64")
	endif()
	foreach(flag
		CMAKE_EXE_LINKER_FLAGS CMAKE_MODULE_LINKER_FLAGS CMAKE_SHARED_LINKER_FLAGS CMAKE_C_FLAGS CMAKE_CXX_FLAGS )
		if(NOT ${flag} MATCHES ${ARCH_FLAG})
			set(${flag} ${${flag}} ${ARCH_FLAG} CACHE "STRING" "Linker flags" FORCE)
		endif()
	endforeach()
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
else(UNIX)
	add_definitions(/MT)
	set(CMAKE_SUPPRESS_REGENERATION 1)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
	target_compile_definitions(${TARGET} PRIVATE _WINDOWS
			_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
	target_compile_options(${TARGET} PRIVATE /utf-8)
endif(UNIX)

add_subdirectory(./GraphViz)
