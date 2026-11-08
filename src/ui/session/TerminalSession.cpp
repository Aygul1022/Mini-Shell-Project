#include "TerminalSession.h"
#include <QDir>
#include <QFileInfo>

TerminalSession::TerminalSession(QWidget* parent) : QTermWidget(0, parent) {
    // Do not start the default system shell before configuring Mini Shell.
    setAutoClose(true);
    connect(this, &QTermWidget::finished, this, [this] {
        active_ = false;
        setEnabled(false);
        emit sessionEnded();
    });
}

bool TerminalSession::start(const QString& executable, const QString& directory,
                            QString& error) {
    error.clear();
    if (started_) {
        error = "This terminal session has already been started.";
        return false;
    }
    const QFileInfo program(executable);
    if (!program.isFile() || !program.isExecutable()) {
        error = "Shell executable is missing or is not executable: " + executable;
        return false;
    }
    if (!QFileInfo(directory).isDir()) {
        error = "Starting directory does not exist: " + directory;
        return false;
    }
    setShellProgram(program.absoluteFilePath());
    setArgs(QStringList{});
    setWorkingDirectory(QDir(directory).absolutePath());
    started_ = true;
    active_ = true;
    // QTermWidget manages the PTY and child; no blocking wait in the GUI.
    startShellProgram();
    setFocus();
    // This reports a launch request, not proof that exec succeeded.
    return true;
}
