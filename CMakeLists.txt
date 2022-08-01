cmake_minimum_required(VERSION 2.4)
project(BrightestDungeon)

set(CMAKE_CXX_STANDARD 17)  
set(CMAKE_CXX_FLAGS "-std=c++17 -lstdc++fs")
set(CMAKE_CXX_FLAGS "-O3")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB_RECURSE SECRET_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/secret/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/secret/*.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/secret/*.c
		${CMAKE_CURRENT_SOURCE_DIR}/secret/*.cpp)

file(GLOB_RECURSE DUNGEON_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(secret ${SECRET_SOURCE_FILES})

add_executable(dungeon ${DUNGEON_SOURCE_FILES})
target_compile_definitions(dungeon PRIVATE NORMAL_ENTRY_POINT SIMULATOR)
target_link_libraries(dungeon ${CURSES_LIBRARIES})

