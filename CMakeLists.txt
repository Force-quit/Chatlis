cmake_minimum_required(VERSION 3.26)
project(Chatlis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENV{PATH} "C:/Qt/6.8.1/msvc2022_64/bin;$ENV{PATH}")
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/msvc2022_64")
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
qt_standard_project_setup()

set(SERVER_SOURCE
    Chatlis-server/Headers/QChatlisServer.h
    Chatlis-server/Headers/QClientConnection.h
    Chatlis-server/Headers/QServerMainWindow.h
    Chatlis-server/Source/main.cpp
    Chatlis-server/Source/QChatlisServer.cpp
    Chatlis-server/Source/QClientConnection.cpp
    Chatlis-server/Source/QServerMainWindow.cpp
)

qt_add_executable(Chatlis-server ${SERVER_SOURCE})

target_link_libraries(Chatlis-server PRIVATE Qt6::Widgets Qt6::Network)
set_target_properties(Chatlis-server PROPERTIES
    WIN32_EXECUTABLE ON
)