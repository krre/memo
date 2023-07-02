find_package(Qt6 REQUIRED COMPONENTS LinguistTools)

if(WIN32)
    set(PLATFORM_SRC ui/hotkey/NativeEventFilterWin.cpp)
endif()

if(UNIX AND NOT APPLE)
    set(PLATFORM_SRC ui/hotkey/NativeEventFilterX11.cpp)
    find_package(X11 REQUIRED)
    set(PLATFORM_LIBS ${X11_LIBRARIES})
endif()

if(APPLE)
    set(PLATFORM_SRC ui/hotkey/NativeEventFilterMac.cpp)
endif()

set(SRC
    core/Application.h core/Application.cpp
    core/Constants.h
    core/Globals.h
    core/Exception.h core/Exception.cpp
    core/MessageHandler.h
    core/Settings.h
    core/SolidString.h core/SolidString.cpp
    database/Database.h database/Database.cpp
    database/Patcher.h database/Patcher.cpp
    database/DatabaseException.h database/DatabaseException.cpp
    server/HttpServerManager.h server/HttpServerManager.cpp
    server/handler/Handler.h server/handler/Handler.cpp
    server/handler/NameHandler.h server/handler/NameHandler.cpp
    server/handler/NotesHandler.h server/handler/NotesHandler.cpp
    ui/MainWindow.h ui/MainWindow.cpp
    ui/TrayIcon.h ui/TrayIcon.cpp
    ui/Editor.h ui/Editor.cpp
    ui/FindText.h ui/FindText.cpp
    ui/Preferences.h ui/Preferences.cpp
    ui/hotkey/GlobalHotkey.h ui/hotkey/GlobalHotkey.cpp
    ui/hotkey/NativeEventFilter.h
    ui/notetaking/NoteProperties.h ui/notetaking/NoteProperties.cpp
    ui/notetaking/NoteTaking.h ui/notetaking/NoteTaking.cpp
    ui/notetaking/TreeItem.h ui/notetaking/TreeItem.cpp
    ui/notetaking/TreeModel.h ui/notetaking/TreeModel.cpp
    i18n/memo-ru.ts
    ${PLATFORM_SRC}
)

add_library(memo-common STATIC ${SRC})

qt_add_translations(memo-common TS_FILES i18n/memo-ru.ts SOURCES ${SRC})

target_link_libraries(memo-common PRIVATE Qt6::Widgets Qt6::Sql Qt6::HttpServer ${PLATFORM_LIBS})
target_include_directories(memo-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Qt6Gui_PRIVATE_INCLUDE_DIRS})
