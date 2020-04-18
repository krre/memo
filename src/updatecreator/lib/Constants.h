#pragma once
#include <QtGlobal>

namespace Const {
    constexpr auto Organization = "Memo";
    constexpr auto ApplicationName = "UpdateCreator";
    constexpr auto WindowTitle = "Memo Update Creator";
    constexpr auto Version = "1.0.0";
    constexpr auto URL = "https://github.com/krre/memo";
    constexpr auto CopyrightYear = "2019-2020";
    constexpr auto ProjectName = "MemoUpdateCreator.json";
    constexpr auto ManifestName = "manifest.json";
    constexpr auto FileTemplate = "memo-update-$version";
    constexpr int SystemRecentProjectsActions = 2;
    constexpr int MaxRecentProjects = 10;

    constexpr auto BuildDate = __DATE__;
    constexpr auto BuildTime = __TIME__;
}
