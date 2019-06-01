#include <Loader.h>
#include <QCoreApplication>
#include <QCommandLineParser>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.process(app);

    QStringList args = parser.positionalArguments();

    if (args.count() != 3) {
        return EXIT_FAILURE;
    }

    Loader loader(args[0], args[1], args[2]);
    loader.start();

    return app.exec();
}
