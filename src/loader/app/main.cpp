#include <Loader.h>
#include <QCoreApplication>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return EXIT_SUCCESS;
    }

    QCoreApplication app(argc, argv);

    Loader loader(argv[1], argv[2], argv[3]);
    loader.start();

    return app.exec();
}
