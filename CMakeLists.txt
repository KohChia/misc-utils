cmake_minimum_required(VERSION 3.10)
project(MiscLibs)

set(CMAKE_BUILD_TYPE "MinSizeRel")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
	add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()

add_executable(
	${CMAKE_PROJECT_NAME}
	iniparser.cpp
	MiscLibs.cpp
)