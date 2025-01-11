################################################
#   _____ ______ _______      ________ _____   #
#  / ____|  ____|  __ \ \    / /  ____|  __ \  #
# | (___ | |__  | |__) \ \  / /| |__  | |__) | #
#  \___ \|  __| |  _  / \ \/ / |  __| |  _  /  #
#  ____) | |____| | \ \  \  /  | |____| | \ \  #
# |_____/|______|_|  \_\  \/   |______|_|  \_\ #
################################################                                            
set(SERVER_SOURCE
    src/QChatlisServer.hpp
    src/QChatlisServer.cpp
    src/QClientConnection.h
    src/QClientConnection.cpp
    src/main.cpp

    src/QServerMainWindow.hpp
    src/QServerMainWindow.cpp

    ../common/NetworkMessage.h
    ../common/QClientInfo.h
    ../common/QClientInfo.cpp
)

qt_add_executable(Chatlis-server ${SERVER_SOURCE})
target_include_directories(Chatlis-server PRIVATE ../common)
target_link_libraries(Chatlis-server PRIVATE Qt6::Widgets Qt6::Network)
set_target_properties(Chatlis-server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/server
)

if(WIN32)
    set_target_properties(Chatlis-server PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

add_subdirectory(resources)

######################################################################
#   _____ ______ _______      ________ _____     _____ _      _____  #
#  / ____|  ____|  __ \ \    / /  ____|  __ \   / ____| |    |_   _| #
# | (___ | |__  | |__) \ \  / /| |__  | |__) | | |    | |      | |   #
#  \___ \|  __| |  _  / \ \/ / |  __| |  _  /  | |    | |      | |   #
#  ____) | |____| | \ \  \  /  | |____| | \ \  | |____| |____ _| |_  #
# |_____/|______|_|  \_\  \/   |______|_|  \_\  \_____|______|_____| #
######################################################################                                                                   

set(SERVER_CLI_SOURCE
    src/QChatlisServer.hpp
    src/QChatlisServer.cpp
    src/QClientConnection.h
    src/QClientConnection.cpp
    src/main-CLI.cpp

    ../common/NetworkMessage.h
    ../common/QClientInfo.h
    ../common/QClientInfo.cpp
)

qt_add_executable(Chatlis-server-CLI ${SERVER_CLI_SOURCE})
target_include_directories(Chatlis-server-CLI PRIVATE ../common)
target_link_libraries(Chatlis-server-CLI PRIVATE Qt6::Network)

set_target_properties(Chatlis-server-CLI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/server
)