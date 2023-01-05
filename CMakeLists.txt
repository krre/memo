cmake_minimum_required(VERSION 3.22)

project(memo VERSION 1.5.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql HttpServer)
qt_standard_project_setup()

add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(memo PRIVATE
    memo1
    Qt6::Widgets)
