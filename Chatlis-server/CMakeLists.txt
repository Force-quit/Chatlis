set(SERVER_SOURCE
    src/QChatlisServer.h
    src/QClientConnection.h
    src/QServerMainWindow.h
    src/main.cpp
    src/QChatlisServer.cpp
    src/QClientConnection.cpp
    src/QServerMainWindow.cpp
    ../common/NetworkMessage.h
    ../common/QClientInfo.h
    ../common/QClientInfo.cpp
)

qt_add_executable(Chatlis-server ${SERVER_SOURCE})
target_include_directories(Chatlis-server PRIVATE ../common)
target_link_libraries(Chatlis-server PRIVATE Qt6::Widgets Qt6::Network)
set_target_properties(Chatlis-server PROPERTIES
    WIN32_EXECUTABLE ON
)