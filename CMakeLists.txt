cmake_minimum_required(VERSION 3.26)
project(Chatlis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	set(ENV{PATH} "C:/Qt/6.8.1/msvc2022_64/bin;$ENV{PATH}")
	set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/msvc2022_64")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
qt_standard_project_setup()

add_subdirectory(Chatlis-server)
add_subdirectory(Chatlis-client)