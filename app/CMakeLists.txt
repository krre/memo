set(SOURCES main.cpp)
qt_add_resources(SOURCES ../src/resources.qrc)
add_executable(memo ${SOURCES})

target_link_libraries(memo PRIVATE common)
