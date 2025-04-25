set(SOURCES main.cpp)

# Icons in resources are taken from https://tabler.io/icons
qt_add_resources(SOURCES ../resources.qrc)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE common)
