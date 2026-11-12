
#pragma once

#include <QMainWindow>
#include <QString>

class QCloseEvent;
class TerminalSession;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    bool startSession(const QString &shellPath,
                      const QString &initialDirectory,
                      QString &error);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    TerminalSession *session_ = nullptr;
};
