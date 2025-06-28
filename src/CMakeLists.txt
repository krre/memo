configure_file(config.h.in config.h @ONLY)

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

add_library(common STATIC
    ${PLATFORM_SRC}
    core/Application.h core/Application.cpp
    core/Exception.h core/Exception.cpp
    core/Exporter.h core/Exporter.cpp
    core/Id.h core/Id.cpp
    core/MessageHandler.h
    core/Model.h
    core/SafeString.h core/SafeString.cpp
    database/Database.h database/Database.cpp
    database/DatabaseException.h database/DatabaseException.cpp
    database/Migrater.h database/Migrater.cpp
    server/HttpServerManager.h server/HttpServerManager.cpp
    server/handler/Handler.h server/handler/Handler.cpp
    server/handler/NameHandler.h server/handler/NameHandler.cpp
    server/handler/NotesHandler.h server/handler/NotesHandler.cpp
    settings/FileSettings.h settings/FileSettings.cpp
    settings/Settings.cpp
    settings/Settings.h
    ui/Birthdays.h ui/Birthdays.cpp
    ui/Editor.h ui/Editor.cpp
    ui/MainWindow.h ui/MainWindow.cpp
    ui/Navigation.h ui/Navigation.cpp
    ui/RecentFilesMenu.h ui/RecentFilesMenu.cpp
    ui/TrayIcon.h ui/TrayIcon.cpp
    ui/dialog/BrowseLayout.cpp ui/dialog/BrowseLayout.h
    ui/dialog/FindAllNotesDialog.h ui/dialog/FindAllNotesDialog.cpp
    ui/dialog/Preferences.h ui/dialog/Preferences.cpp
    ui/dialog/StandardDialog.h ui/dialog/StandardDialog.cpp
    ui/hotkey/GlobalHotkey.h ui/hotkey/GlobalHotkey.cpp
    ui/hotkey/NativeEventFilter.h
    ui/notetaking/NoteProperties.h ui/notetaking/NoteProperties.cpp
    ui/notetaking/NoteTaking.h ui/notetaking/NoteTaking.cpp
    ui/notetaking/TreeItem.h ui/notetaking/TreeItem.cpp
    ui/notetaking/TreeModel.h ui/notetaking/TreeModel.cpp
)

# For visibility in IDE (e.g., Qt Creator)
add_custom_target(custom
    SOURCES
    config.h.in
)

target_link_libraries(common PUBLIC Qt6::Widgets Qt6::Sql Qt6::HttpServer ${PLATFORM_LIBS})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Qt6Gui_PRIVATE_INCLUDE_DIRS})
