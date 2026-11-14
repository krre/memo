cmake_minimum_required(VERSION 4.3)

project(memo VERSION 1.18.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(REQUIRED_QT_VERSION 6.11.1)
set(QT_NO_PRIVATE_MODULE_WARNING ON)

find_package(Qt6 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS
    Widgets
    Sql
    HttpServer
    LinguistTools
    CorePrivate
)

qt_standard_project_setup(
    REQUIRES ${REQUIRED_QT_VERSION}
    I18N_TRANSLATED_LANGUAGES ru
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

enable_testing()

add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(tests)

option(MEMO_LUPDATE_NO_OBSOLETE "Remove obsolete translations on lupdate" OFF)

set(_lupdate_opts "")

if(MEMO_LUPDATE_NO_OBSOLETE)
    list(APPEND _lupdate_opts -no-obsolete)
endif()

qt_add_translations(${PROJECT_NAME}
    MERGE_QT_TRANSLATIONS
    NO_GENERATE_PLURALS_TS_FILE
    TS_OUTPUT_DIRECTORY i18n
    LUPDATE_OPTIONS ${_lupdate_opts}
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
    NO_TRANSLATIONS
)

install(SCRIPT ${deploy_script})
