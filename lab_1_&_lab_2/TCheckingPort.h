#ifndef TCHECKINGPORT_H
#define TCHECKINGPORT_H

#include <QObject>
#include <QThread>
#include <comport.h>

class TCheckingPort : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)
    bool m_isRunning;

public:
    explicit TCheckingPort(QObject *parent = nullptr);
    TCheckingPort(COMPort* port, QObject *parent = nullptr);

    void setPort(COMPort* port);
    void runCheckingPort();
    void runPackageCatching();

    bool isRunning() const;

signals:
    void isRunningChanged(bool isRunning);
    void informationIsHere(QString);
    void finished();

public slots:
    void setIsRunning(bool isRunning);

private:
    COMPort* port;
};

#endif // TCHECKINGPORT_H
