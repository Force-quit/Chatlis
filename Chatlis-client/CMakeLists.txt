set(CLIENT_SOURCE
    src/main.cpp
    src/QChatbox.cpp
    src/QChatbox.hpp
    src/QChatlisMenuBar.cpp
    src/QChatlisMenuBar.hpp
    src/QChatRoomMainWindow.cpp
    src/QChatRoomMainWindow.hpp
    src/QChatWidget.cpp
    src/QChatWidget.hpp
    src/QParticipantsPanel.cpp
    src/QParticipantsPanel.hpp
    src/QServerConnection.cpp
    src/QServerConnection.hpp
    src/QParticipantLabel.hpp

    ../common/NetworkMessage.hpp
    ../common/QClientInfo.hpp
    ../common/QClientInfo.cpp
)

qt_add_executable(Chatlis-client ${CLIENT_SOURCE})
target_include_directories(Chatlis-client PRIVATE ../common)
target_link_libraries(Chatlis-client PRIVATE Qt6::Widgets Qt6::Network)
set_target_properties(Chatlis-client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/client
)

if(WIN32)
    set_target_properties(Chatlis-client PROPERTIES WIN32_EXECUTABLE ON)
endif()

add_subdirectory(resources)