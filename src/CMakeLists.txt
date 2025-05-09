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
    ui/dialog/StandardDialog.h ui/dialog/StandardDialog.cpp
    core/Application.h core/Application.cpp
    core/Exception.h core/Exception.cpp
    core/MessageHandler.h
    core/SafeString.h core/SafeString.cpp
    database/Database.h database/Database.cpp
    database/Migrater.h database/Migrater.cpp
    database/DatabaseException.h database/DatabaseException.cpp
    server/HttpServerManager.h server/HttpServerManager.cpp
    server/handler/Handler.h server/handler/Handler.cpp
    server/handler/NameHandler.h server/handler/NameHandler.cpp
    server/handler/NotesHandler.h server/handler/NotesHandler.cpp
    ui/MainWindow.h ui/MainWindow.cpp
    ui/RecentFilesMenu.h ui/RecentFilesMenu.cpp
    ui/TrayIcon.h ui/TrayIcon.cpp
    ui/Editor.h ui/Editor.cpp
    ui/dialog/FindAllNotesDialog.h ui/dialog/FindAllNotesDialog.cpp
    ui/dialog/Preferences.h ui/dialog/Preferences.cpp
    ui/hotkey/GlobalHotkey.h ui/hotkey/GlobalHotkey.cpp
    ui/hotkey/NativeEventFilter.h
    ui/notetaking/NoteProperties.h ui/notetaking/NoteProperties.cpp
    ui/notetaking/NoteTaking.h ui/notetaking/NoteTaking.cpp
    ui/notetaking/TreeItem.h ui/notetaking/TreeItem.cpp
    ui/notetaking/TreeModel.h ui/notetaking/TreeModel.cpp
    ui/Birthdays.h ui/Birthdays.cpp
    core/Exporter.h core/Exporter.cpp
    settings/Settings.h
    settings/FileSettings.h settings/FileSettings.cpp
    core/Model.h
    ui/dialog/BrowseLayout.cpp ui/dialog/BrowseLayout.h
    settings/Settings.cpp
    core/Id.h core/Id.cpp
    ui/Navigation.h ui/Navigation.cpp
)

# For visibility in IDE (e.g., Qt Creator)
add_custom_target(custom
    SOURCES
    config.h.in
)

target_link_libraries(common PUBLIC Qt6::Widgets Qt6::Sql Qt6::HttpServer ${PLATFORM_LIBS})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Qt6Gui_PRIVATE_INCLUDE_DIRS})
