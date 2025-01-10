set(CLIENT_SOURCE
    src/main.cpp
    src/QChatbox.cpp
    src/QChatbox.h
    src/QChatlisMenuBar.cpp
    src/QChatlisMenuBar.h
    src/QChatRoomMainWindow.cpp
    src/QChatRoomMainWindow.h
    src/QChatWidget.cpp
    src/QChatWidget.h
    src/QParticipantsPanel.cpp
    src/QParticipantsPanel.h
    src/QServerConnection.cpp
    src/QServerConnection.h

    ../common/NetworkMessage.h
    ../common/QClientInfo.h
    ../common/QClientInfo.cpp
)

qt_add_executable(Chatlis-client ${CLIENT_SOURCE})
target_include_directories(Chatlis-client PRIVATE ../common)
target_link_libraries(Chatlis-client PRIVATE Qt6::Widgets Qt6::Network)
set_target_properties(Chatlis-client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/client
)

if(WIN32)
    set_target_properties(Chatlis-client PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

add_subdirectory(resources)