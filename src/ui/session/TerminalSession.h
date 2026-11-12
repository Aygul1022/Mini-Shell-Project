#pragma once

#include <qtermwidget.h>

// Embeddable terminal component: the host owns layout and visual design.
class TerminalSession : public QTermWidget {
    Q_OBJECT
public:
    explicit TerminalSession(QWidget* parent = nullptr);
    bool start(const QString& executable, const QString& directory, QString& error);
    bool isActive() const { return active_; }

signals:
    void sessionEnded();

private:
    bool started_ = false;
    bool active_ = false;
};
