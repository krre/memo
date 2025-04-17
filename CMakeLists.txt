cmake_minimum_required(VERSION 3.22)

project(memo VERSION 1.11.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    Sql
    HttpServer
    LinguistTools
)

qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES ru)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(tests)

qt_add_translations(${PROJECT_NAME}
    NO_GENERATE_PLURALS_TS_FILE
    TS_OUTPUT_DIRECTORY i18n
    # LUPDATE_OPTIONS -no-obsolete
)

# For visibility in IDE (e.g., Qt Creator)
add_custom_target(other
    SOURCES
    README.md
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

