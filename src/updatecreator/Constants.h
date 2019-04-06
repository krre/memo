#pragma once
#include <QtGlobal>

namespace Constants {
    constexpr auto Organization = "Memo";
    constexpr auto ApplicationName = "UpdateCreator";
    constexpr auto WindowTitle = "Memo Update Creator";
    constexpr auto Version = "1.0.0";
    constexpr auto URL = "https://github.com/krre/memo";
    constexpr auto CopyrightYear = "2019";
    constexpr auto ProjectName = "MemoUpdateCreator.json";
    constexpr auto ManifestName = "manifest.json";
    constexpr auto FileTemplate = "memo-update-$version";
    constexpr int SystemRecentProjectsActions = 2;
    constexpr int MaxRecentProjects = 10;

#ifdef Q_OS_WIN
    constexpr auto CurrentOS = "windows";
#elif defined (Q_OS_LINUX)
    constexpr auto CurrentOS = "linux";
#elif defined (Q_OS_MACOS)
    constexpr auto CurrentOS = "macos";
#endif
}
