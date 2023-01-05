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

add_library(memo1 STATIC
    core/Constants.h
    core/Globals.h
    core/DatabaseException.h core/DatabaseException.cpp
    core/Exception.h core/Exception.cpp
    core/MessageHandler.h
    core/Settings.h core/Settings.cpp
    database/Database.h database/Database.cpp
    database/Patcher.h database/Patcher.cpp
    server/HttpServer.h server/HttpServer.cpp
    ui/MainWindow.h ui/MainWindow.cpp
    ui/Editor.h ui/Editor.cpp
    ui/Preferences.h ui/Preferences.cpp
    ui/hotkey/GlobalHotkey.h ui/hotkey/GlobalHotkey.cpp
    ui/hotkey/NativeEventFilter.h
    ui/notetaking/NoteProperties.h ui/notetaking/NoteProperties.cpp
    ui/notetaking/NoteTaking.h ui/notetaking/NoteTaking.cpp
    ui/notetaking/TreeItem.h ui/notetaking/TreeItem.cpp
    ui/notetaking/TreeModel.h ui/notetaking/TreeModel.cpp
    ${PLATFORM_SRC}
)

target_link_libraries(memo1 PRIVATE Qt6::Widgets Qt6::Sql Qt6::HttpServer ${PLATFORM_LIBS})
target_include_directories(memo1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Qt6Gui_PRIVATE_INCLUDE_DIRS})
