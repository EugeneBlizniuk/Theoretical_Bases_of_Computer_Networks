#include "comport.h"

bool COMPort::connectCOM(string port, const unsigned int baud) {
    portName = port;

    disconnectCOM();
    qDebug() << "port: " << port.c_str() << "baud: " << baud <<"\n";

    hPort = CreateFileA(
        port.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    qDebug() << "Port handle: " << hPort << "\n";

    if (hPort == INVALID_HANDLE_VALUE) {
        hPort = nullptr;
        return false;
    }

    SetupComm(hPort, 1500, 1500);

    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = TIMEOUT;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;

    if (!SetCommTimeouts(hPort, &CommTimeOuts)) {
        hPort = nullptr;
        return false;
    }

    DCB ComDCM;
    memset(&ComDCM, 0, sizeof(ComDCM));
    ComDCM.DCBlength = sizeof(DCB);
    GetCommState(hPort, &ComDCM);
    ComDCM.BaudRate = DWORD(baud);
    ComDCM.ByteSize = 8;
    ComDCM.Parity = NOPARITY;
    ComDCM.StopBits = ONESTOPBIT;
    ComDCM.fAbortOnError = TRUE;
    ComDCM.fDtrControl = DTR_CONTROL_DISABLE;

    ComDCM.fRtsControl = RTS_CONTROL_TOGGLE;
    ComDCM.fBinary = TRUE;
    ComDCM.fParity = FALSE;
    ComDCM.fInX = ComDCM.fOutX = FALSE;
    ComDCM.XonChar = 0;
    ComDCM.XoffChar = uint8_t(0xff);
    ComDCM.fErrorChar = FALSE;
    ComDCM.fNull = FALSE;
    ComDCM.fOutxCtsFlow = FALSE;
    ComDCM.fOutxDsrFlow = FALSE;
    ComDCM.XonLim = 128;
    ComDCM.XoffLim = 128;

    if (!SetCommState(hPort, &ComDCM)) {
        CloseHandle(hPort);
        hPort = nullptr;
        return false;
    }

    return true;
}

void COMPort::disconnectCOM() {
    if(hPort != nullptr) {
        CloseHandle(hPort);
        hPort = nullptr;
    }
}

HANDLE COMPort::getHPort() const {
    return hPort;
}

unsigned int COMPort::getBaudrate() const {
    return baudrate;
}

void COMPort::setBaudrate(const unsigned int baudrate) {
    if(portName == "") {
        return;
    }

    disconnectCOM();
    this->baudrate = baudrate;
    connectCOM(portName, baudrate);
}

bool COMPort::writeToCOM(string information) {
    if(isPortAvailable()) {
        DWORD feedback;

        if(!WriteFile(hPort, information.c_str(), information.length(), &feedback, nullptr)
                || feedback != information.length()) {
           return false;
        }

        return true;
    }

    return false;
}

bool COMPort::readFromCOM(string &information) {
    if(isPortAvailable()) {
        char buffer[FEEDBACK_SIZE];
        DWORD feedback = 0;

        ReadFile(hPort, buffer, FEEDBACK_SIZE, &feedback, nullptr);

        if(feedback == 0) {
            return false;
        }

        buffer[feedback] = '\0';
        information = string(buffer);

        return true;
    }

    return false;
}


bool COMPort::isPortAvailable() {
    return hPort != nullptr ? true : false;
}

bool COMPort::writePackages(string information) {
    if(isPortAvailable()) {
        DWORD feedback;
        information = encryptData(information); // 2
        if(!WriteFile(hPort, information.c_str(), information.length(), &feedback, nullptr)
                || feedback != information.length()) {
           return false;
        }
        return true;
    }
    return false;
}

bool COMPort::readPackages(std::string &information) {
    if(isPortAvailable()) {
        char buffer[FEEDBACK_SIZE];
        DWORD feedback = 0;

        ReadFile(hPort, buffer, FEEDBACK_SIZE, &feedback, nullptr);

        if(feedback == 0) {
            return false;
        }

        buffer[feedback] = '\0';
        qDebug() << "buffer: " << buffer << "\n";
        string currentStr = decryptData(buffer); //2
        qDebug() << "currentStr: " << currentStr.c_str() << "\n";
        information = currentStr;

        return true;
    }

    return false;
}

string COMPort::encryptData(string data) {
    string result;
    qDebug() << "Data is entered: " << data.c_str() << "\n";
    data = FLAG_SEQUENCE + data + FLAG_SEQUENCE;

    for(unsigned int i = 0; i < data.length(); i++) {

        if(data.at(i) == 'F' && i != 0 && i != (data.length()) - 1) {
            result = result + 'E' + data.at(i);
        } else if(data.at(i) == 'E') {
            result = result + 'E' + data.at(i);
        } else {
            result = result + data.at(i);
        }
    }

    qDebug() << "Encrypt data: " << result.c_str() << "\n";

    return result;
}

string COMPort::decryptData(string data) {
    qDebug() << "Data came: " << data.c_str() << "\n";
    string decryptString;

    for(unsigned int i = 1; i < data.length() - 1; i++) {

        if(data.at(i) == 'D' || data.at(i) == 'F') {
            decryptString = decryptString + data.at(i);
        } else if(data.at(i) == 'E' && data.at(i + 1) == 'E') {
            decryptString = decryptString + 'E';
            i++;
        } else if(data.at(i) == 'E' && data.at(i+1) == 'F') {
            decryptString = decryptString + 'F';
            i++;
        } else {
            decryptString = decryptString + data.at(i);
        }
    }

    qDebug() << "Decrypt data: " << decryptString.c_str() << "\n";

    return decryptString;
}










