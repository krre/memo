#pragma once

namespace Constants {
    namespace App {
        constexpr auto Organization = "Memo";
        constexpr auto Name = "Memo";
        constexpr auto Version = "1.2.0";
        constexpr auto Status = ""; // Release, Beta, Alpha
        constexpr auto URL = "https://github.com/krre/memo";
        constexpr auto ReleasesUrl = "https://github.com/krre/memo/releases";
        constexpr auto DownloadUrl = "http://memo.inf.ua/download";
        constexpr auto CopyrightYear = "2019";
        constexpr auto BuildDate = __DATE__;
        constexpr auto BuildTime = __TIME__;
    }

    namespace Window {
        constexpr int MaxRecentFiles = 10;
        constexpr int SystemRecentFilesActions = 2;
    }

    namespace Outliner {
        constexpr auto TreeItemMimeType = "application/x-treeitem";
    }

    namespace DefaultSettings {
        constexpr auto GlobalHotkey = "Ctrl+Alt+M";
        constexpr bool CheckOnStartup = true;
        constexpr auto Channel = "release";
    }
}
