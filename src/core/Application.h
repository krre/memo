#pragma once
#include <QApplication>

class Application : public QApplication {
public:
    Application(int& argc, char* argv[]);
    bool setup();

private:
    void installTranslators();
};
