set(SERVER_SOURCE
    src/QChatlisServer.h
    src/QChatlisServer.cpp
    src/main.cpp

    src/QClientConnection.h
    src/QServerMainWindow.h
    src/QClientConnection.cpp
    src/QServerMainWindow.cpp
    ../common/NetworkMessage.h
    ../common/QClientInfo.h
    ../common/QClientInfo.cpp
)

set(SERVER_CLI_SOURCE
    src/QChatlisServer-CLI.h
    src/QChatlisServer-CLI.cpp
    src/main-CLI.cpp

    src/QClientConnection.h
    src/QClientConnection.cpp
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

qt_add_executable(Chatlis-server-CLI ${SERVER_CLI_SOURCE})
target_include_directories(Chatlis-server-CLI PRIVATE ../common)
target_link_libraries(Chatlis-server-CLI PRIVATE Qt6::Network)