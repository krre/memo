#pragma once

namespace Const {

namespace App {
    constexpr auto Organization = "Memo";
    constexpr auto Name = "Memo";
    constexpr auto Version = "1.6.0";
    constexpr auto Status = ""; // Release, Beta, Alpha
    constexpr auto URL = "https://github.com/krre/memo-desktop";
    constexpr auto ReleasesUrl = "https://github.com/krre/memo-desktop/releases";
    constexpr auto CopyrightYear = "2019-2023";
    constexpr auto BuildDate = __DATE__;
    constexpr auto BuildTime = __TIME__;
}

namespace Window {
    constexpr int MaxRecentFiles = 10;
    constexpr int SystemRecentFilesActions = 2;
}

namespace NoteTaking {
    constexpr auto TreeItemMimeType = "application/x-treeitem";
}

}
