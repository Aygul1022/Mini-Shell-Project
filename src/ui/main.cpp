
#include "MainWindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // By default, find the shell beside the GUI executable.
    QString shellPath =
        QCoreApplication::applicationDirPath() + "/mini-shell";

    QString initialDirectory = QDir::currentPath();

    // Allow a custom shell path and working directory.
    if (argc > 1) {
        shellPath = QString::fromLocal8Bit(argv[1]);
    }

    if (argc > 2) {
        initialDirectory = QString::fromLocal8Bit(argv[2]);
    }

    MainWindow window;
    window.show();

    QString error;

    if (!window.startSession(shellPath, initialDirectory, error)) {
        QMessageBox::critical(
            &window,
            "Mini-Shell Startup Error",
            error
        );

        return 1;
    }

    return app.exec();
}
