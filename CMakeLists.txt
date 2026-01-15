cmake_minimum_required(VERSION 4.1.1)
project(Chatlis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
qt_standard_project_setup()

add_subdirectory(Chatlis-server)
add_subdirectory(Chatlis-client)