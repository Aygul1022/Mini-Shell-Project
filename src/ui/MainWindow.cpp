
#include "MainWindow.h"
#include "session/TerminalSession.h"

#include <qtermwidget.h>

#include <QAction>
#include <QCloseEvent>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      session_(new TerminalSession(this))
{
    setWindowTitle("Linux Mini-Shell");
    resize(960, 640);

    // Embed the terminal in the main window.
    setCentralWidget(session_);

    statusBar()->showMessage("Terminal is ready.");

    // Add a menu for terminal clipboard operations.
    QMenu *editMenu = menuBar()->addMenu("&Edit");

    QAction *copyAction = editMenu->addAction("Copy");
    
copyAction->setShortcuts({
    QKeySequence("Ctrl+C"),
    QKeySequence("Ctrl+Shift+C")
});

    copyAction->setShortcutContext(Qt::ApplicationShortcut);

    connect(copyAction, &QAction::triggered,
            session_->terminalWidget(),
            &QTermWidget::copyClipboard);

    QAction *pasteAction = editMenu->addAction("Paste");
    pasteAction->setShortcuts({
    QKeySequence("Ctrl+Shift+V"),
    QKeySequence("Ctrl+V")
});
    pasteAction->setShortcutContext(Qt::ApplicationShortcut);

    connect(pasteAction, &QAction::triggered,
            session_->terminalWidget(),
            &QTermWidget::pasteClipboard);

    // Handle shell session termination.
    
connect(session_, &TerminalSession::sessionEnded,
        this, [this]() {
            statusBar()->showMessage("Shell session ended.");
            close();
        });

}

bool MainWindow::startSession(
    const QString &shellPath,
    const QString &initialDirectory,
    QString &error)
{
    if (!session_->start(shellPath, initialDirectory, error)) {
        return false;
    }

    statusBar()->showMessage("Shell session running.");
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (session_ && session_->isRunning()) {
        QMessageBox::warning(
            this,
            "Active Session",
            "Please type 'exit' in the terminal "
            "before closing this window."
        );

        event->ignore();
        return;
    }

    QMainWindow::closeEvent(event);
}
