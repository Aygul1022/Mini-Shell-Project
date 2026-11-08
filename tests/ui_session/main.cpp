#include "TerminalSession.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QMainWindow>
#include <QMessageBox>
#include <QStatusBar>
#include <QTimer>
#include <iostream>

class SessionTestWindow : public QMainWindow {
public:
    TerminalSession* terminal;
    SessionTestWindow() : terminal(new TerminalSession(this)) {
        setWindowTitle("Mini Shell - session prototype");
        resize(900, 550);
        setCentralWidget(terminal);
        statusBar()->showMessage("Starting session");
        connect(terminal, &TerminalSession::sessionEnded, this, [this] {
            statusBar()->showMessage("Session ended. You can close this window.");
        });
    }
protected:
    void closeEvent(QCloseEvent* event) override {
        if (terminal->isActive()) {
            statusBar()->showMessage("Type exit at the shell prompt before closing.");
            event->ignore();
            return;
        }
        event->accept();
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    if (app.arguments().size() != 2) {
        std::cerr << "Usage: minishell-session-test /absolute/path/to/mini-shell\n";
        return 2;
    }
    SessionTestWindow window;
    window.show();
    QTimer::singleShot(0, &window, [&] {
        QString error;
        if (!window.terminal->start(app.arguments().at(1), QDir::currentPath(), error)) {
            QMessageBox::critical(&window, "Cannot start Mini Shell", error);
            app.exit(1);
        } else if (window.terminal->isActive()) {
            window.statusBar()->showMessage("Session launch requested");
        }
    });
    return app.exec();
}
