file(COPY SSL DESTINATION ${CMAKE_BINARY_DIR}/server)

target_sources(Chatlis-server PRIVATE server.rc)
qt_add_resources(Chatlis-server imageresources
    PREFIX "/images"
    FILES server.png
)