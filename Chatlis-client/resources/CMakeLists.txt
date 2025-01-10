file(COPY SSL DESTINATION ${CMAKE_BINARY_DIR}/client)

target_sources(Chatlis-client PRIVATE client.rc)
qt_add_resources(Chatlis-client imageresources
    PREFIX "/images"
    FILES client.png
)