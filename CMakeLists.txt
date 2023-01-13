cmake_minimum_required(VERSION 3.22)

project(memo VERSION 1.5.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_RPATH $ORIGIN:$ORIGIN/../lib)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql HttpServer)
qt_standard_project_setup()

add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(memo PRIVATE memo-common)
target_link_libraries(test_settings PRIVATE memo-common)

install(TARGETS memo
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET memo
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

qt_finalize_target(memo)

install(SCRIPT ${deploy_script})

set(CPACK_PACKAGE_NAME memo)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Note-taking")
set(CPACK_PACKAGE_VENDOR "Memo Group")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_VERBATIM_VARIABLES ON)
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/memo")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Vladimir Zarypov <krre31@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS libc6 libstdc++6 libgcc-s1)
include(CPack)
