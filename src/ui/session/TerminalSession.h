
#pragma once

#include <QString>
#include <QWidget>

class QTermWidget;

class TerminalSession : public QWidget
{
    Q_OBJECT

public:
    explicit TerminalSession(QWidget *parent = nullptr);

    bool start(const QString &shellPath,
               const QString &initialDirectory,
               QString &error);

    bool isRunning() const;

    QTermWidget *terminalWidget() const;

signals:
    void sessionEnded();

private:
    QTermWidget *terminal_ = nullptr;
    bool started_ = false;
    bool running_ = false;
};
