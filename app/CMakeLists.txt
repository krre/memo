qt_add_executable(${PROJECT_NAME}
    main.cpp
    # Icons in resources are taken from https://tabler.io/icons
    ../resources.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE common)
