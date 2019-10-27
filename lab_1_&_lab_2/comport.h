#ifndef COMPORT_H
#define COMPORT_H
#define TIMEOUT 100
#define FEEDBACK_SIZE 1024

#include <windows.h>
#include <iostream>
#include <QDebug>
#include <QStringList>

using std::string;
#define FLAG_SEQUENCE 'F'
#define ESCAPE_SEQUENCE 'E'

class COMPort {
public slots:
    COMPort() {}
    ~COMPort() {}

    bool connectCOM(string port, const unsigned int baud);
    void disconnectCOM();

    HANDLE getHPort() const;
    unsigned int getBaudrate() const;

    void setBaudrate(const unsigned int baudrate);

    bool writeToCOM(string information);
    bool readFromCOM(string& information);

    bool writePackages(string information);
    bool readPackages(string& information);
private slots:
    bool isPortAvailable();

    string encryptData(string data);
    string decryptData(string data);
private:
    string portName = "";
    HANDLE hPort = nullptr;
    unsigned int baudrate = 0;
};

#endif // COMPORT_H
