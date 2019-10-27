#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <TCheckingPort.h>

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

public slots:
    void putMessage(QString);

private slots:
    void on_initialize_clicked();

    void on_comNameBox_activated(const QString &comName);
    void setComName(QString);
    void on_baudrateBox_activated(const QString &baudrate);
    void setBaudrate(QString);

    void on_sendButton_clicked();

    void on_sendPackagesButton_clicked();

private:
    Ui::Window *ui;

    QThread portThread;
    QThread portThreadForPackage;
    TCheckingPort threadChecker;
    TCheckingPort threadCheckerForPackage;

    COMPort comPort;
    string comName = "";
    unsigned int comBaudrate = 0;
};

#endif // WINDOW_H
