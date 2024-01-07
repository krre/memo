#pragma once
#include "config.h"
#include <QApplication>

class Application : public QApplication {
public:
    static constexpr auto Organization = "Memo";
    static constexpr auto Name = "Memo";
    static constexpr auto Version = PROJECT_VERSION;
    static constexpr auto Status = ""; // Release, Beta, Alpha
    static constexpr auto Url = "https://github.com/krre/memo";
    static constexpr auto ReleasesUrl = "https://github.com/krre/memo/releases";
    static constexpr auto CopyrightYear = "2019-2024";
    static constexpr auto BuildDate = __DATE__;
    static constexpr auto BuildTime = __TIME__;

    Application(int& argc, char* argv[]);

private:
    void installTranslators();
};
