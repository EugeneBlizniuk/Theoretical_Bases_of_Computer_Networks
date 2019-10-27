#include "TCheckingPort.h"

TCheckingPort::TCheckingPort(QObject *parent) : QObject(parent) {

}

TCheckingPort::TCheckingPort(COMPort* port, QObject *parent) : QObject (parent){
    this->port = port;
}

void TCheckingPort::setPort(COMPort* port) {
    this->port = port;
}

void TCheckingPort::runCheckingPort() {
    string information;

    while (m_isRunning) {
        QThread::msleep(10);

        if(port->readFromCOM(information)) {
            emit informationIsHere(QString::fromStdString(information));
        }
    }

    emit finished();
}

void TCheckingPort::runPackageCatching() {
    string information;

    while (m_isRunning) {
        QThread::msleep(10);

        if(port->readPackages(information)) {
            emit informationIsHere(QString::fromStdString(information));
        }
    }

    emit finished();
}

bool TCheckingPort::isRunning() const {
    return m_isRunning;
}

void TCheckingPort::setIsRunning(bool isRunning) {
    if (m_isRunning == isRunning) {
        return;
    }

    m_isRunning = isRunning;
    emit isRunningChanged(m_isRunning);
}
