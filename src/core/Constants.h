#pragma once

namespace Constants {
    namespace App {
        constexpr auto Organization = "Memo";
        constexpr auto Name = "Memo";
        constexpr auto Version = "1.2";
        constexpr auto Status = ""; // Release, Beta, Alpha
        constexpr auto URL = "https://github.com/krre/memo";
        constexpr auto SettingsName = "memo.conf";
        constexpr auto CopyrightYear = "2019";
    }

    namespace Window {
        constexpr int MaxRecentFiles = 10;
        constexpr int SystemRecentFilesActions = 2;
        constexpr auto DefaultGlobalHotkey = "Ctrl+Alt+M";
    }

    namespace Outliner {
        constexpr auto TreeItemMimeType = "application/x-treeitem";
    }

    namespace Updater {
        constexpr auto ManifestRedirectorUrl = "https://raw.githubusercontent.com/krre/memo/master/src/updater/redirector.json";
    }
}
