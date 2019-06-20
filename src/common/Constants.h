#pragma once
#include <QtGlobal>

namespace Common {
namespace Constants {
namespace Channel {
    constexpr auto Alpha = "alpha";
    constexpr auto Beta = "beta";
    constexpr auto Release = "release";
}

namespace OS {
    constexpr auto Windows = "windows";
    constexpr auto Linux = "linux";
    constexpr auto MacOS = "macos";
}

#ifdef Q_OS_WIN
    constexpr auto CurrentOS = OS::Windows;
#elif defined (Q_OS_LINUX)
    constexpr auto CurrentOS = OS::Linux;
#elif defined (Q_OS_MACOS)
    constexpr auto CurrentOS = OS::MacOS;
#endif

} // namespace Constants
} // namespace Common
