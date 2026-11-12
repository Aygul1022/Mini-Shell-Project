
#include "TerminalSession.h"

#include <qtermwidget.h>

#include <QDir>
#include <QFileInfo>
#include <QVBoxLayout>

#include <QFont>

TerminalSession::TerminalSession(QWidget *parent)
    : QWidget(parent),
      terminal_(new QTermWidget(0, this))
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(terminal_);

    // Keep the terminal visible after the shell exits.
    terminal_->setAutoClose(true);
    terminal_->setHistorySize(1000);

terminal_->setColorScheme("WhiteOnBlack");
terminal_->setTerminalFont(QFont("Monospace", 12));


    // Notify the main window when the shell session ends.
    connect(terminal_, &QTermWidget::finished,
            this, [this]() {
                running_ = false;
                emit sessionEnded();
            });
}

bool TerminalSession::start(const QString &shellPath,
                            const QString &initialDirectory,
                            QString &error)
{
    error.clear();

    if (started_) {
        error = "The terminal session has already been started.";
        return false;
    }

    QFileInfo shellFile(shellPath);

    if (!shellFile.isFile()) {
        error = "Shell executable not found: " + shellPath;
        return false;
    }

    if (!shellFile.isExecutable()) {
        error = "Shell file is not executable: " + shellPath;
        return false;
    }

    const QString directory = initialDirectory.isEmpty()
        ? QDir::currentPath()
        : initialDirectory;

    if (!QDir(directory).exists()) {
        error = "Working directory not found: " + directory;
        return false;
    }

    terminal_->setShellProgram(shellFile.absoluteFilePath());
    terminal_->setWorkingDirectory(directory);

    started_ = true;
    running_ = true;

    // Launch the existing Mini-Shell through QTermWidget's PTY.
    terminal_->startShellProgram();

    return true;
}

bool TerminalSession::isRunning() const
{
    return running_;
}

QTermWidget *TerminalSession::terminalWidget() const
{
    return terminal_;
}
