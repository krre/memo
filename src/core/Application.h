#pragma once
#include "src/config.h"
#include <QApplication>

class Application : public QApplication {
public:
    static constexpr auto Organization = "Memo";
    static constexpr auto Name = "Memo";
    static constexpr auto Version = PROJECT_VERSION;
    inline static const auto Url = QStringLiteral("https://github.com/krre/memo");
    inline static const auto ReleasesUrl = Url + QStringLiteral("/releases");
    static constexpr auto Years = "2019-2026";
    static constexpr auto BuildDate = __DATE__;
    static constexpr auto BuildTime = __TIME__;

    Application(int& argc, char* argv[]);

private:
    void installTranslators();
};
