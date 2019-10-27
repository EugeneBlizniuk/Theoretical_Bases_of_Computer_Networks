#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window) {
    ui->setupUi(this);

//    threadChecker.setPort(&comPort);
    threadCheckerForPackage.setPort(&comPort);

//    connect(&portThread, &QThread::started, &threadChecker, &TCheckingPort::runCheckingPort);
//    connect(&threadChecker, &TCheckingPort::finished, &portThread, &QThread::terminate);
//    connect(&threadChecker, SIGNAL(informationIsHere(QString)), this, SLOT(putMessage(QString)));

//    threadChecker.moveToThread(&portThread);

//    portThread.start();
//    threadChecker.setIsRunning(true); // may be when initializating

    connect(&portThreadForPackage, &QThread::started, &threadCheckerForPackage, &TCheckingPort::runPackageCatching);
    connect(&threadCheckerForPackage, &TCheckingPort::finished, &portThreadForPackage, &QThread::terminate);
    connect(&threadCheckerForPackage, SIGNAL(informationIsHere(QString)), this, SLOT(putMessage(QString)));

    threadCheckerForPackage.moveToThread(&portThreadForPackage);

    portThreadForPackage.start();
    threadCheckerForPackage.setIsRunning(true); // may be when initializating

    ui->comNameBox->addItem("");
    ui->comNameBox->addItem("COM1");
    ui->comNameBox->addItem("COM2");

    ui->baudrateBox->addItem("");
    ui->baudrateBox->addItem("110");
    ui->baudrateBox->addItem("300");
    ui->baudrateBox->addItem("600");
    ui->baudrateBox->addItem("1200");
    ui->baudrateBox->addItem("2400");
    ui->baudrateBox->addItem("4800");
    ui->baudrateBox->addItem("9600");
    ui->baudrateBox->addItem("14400");
    ui->baudrateBox->addItem("19200");
    ui->baudrateBox->addItem("38400");
    ui->baudrateBox->addItem("56000");
    ui->baudrateBox->addItem("57600");
    ui->baudrateBox->addItem("115200");
}

Window::~Window() {
    delete ui;

//    threadChecker.setIsRunning(false);
    threadCheckerForPackage.setIsRunning(false);
}

void Window::putMessage(QString message) {
    ui->listToGet->setTextColor(Qt::blue);
    ui->listToGet->append(message);
    ui->listToGet->setTextColor(Qt::black);
}

void Window::on_initialize_clicked() {
    if(comName != "" && comBaudrate != 0) {
        if(comPort.connectCOM(comName, comBaudrate)) {
            ui->listToGet->setTextColor(Qt::green);
            ui->listToGet->append("Connected");
            ui->listToGet->setTextColor(Qt::black);
        } else {
            ui->listToGet->setTextColor(Qt::red);
            ui->listToGet->append("Uups...Something is going wrong, check your port!");
            ui->listToGet->setTextColor(Qt::black);
        } // 2nd if
    } else if(comName == "") {
        ui->listToGet->setTextColor(Qt::darkYellow);
        ui->listToGet->append("Put your port name!");
        ui->listToGet->setTextColor(Qt::black);
    } else if(comBaudrate == 0) {
        ui->listToGet->setTextColor(Qt::darkRed);
        ui->listToGet->append("Put your baudrate!");
        ui->listToGet->setTextColor(Qt::black);
    } // 1st if
}

void Window::on_comNameBox_activated(const QString &comName) {
    setComName(comName);
}

void Window::setComName(QString name) {
    string currentString = name.toStdString();

    if(comName == currentString) {
        return;
    }

    comName = currentString;
}

void Window::on_baudrateBox_activated(const QString &baudrate) {
    setBaudrate(baudrate);
}

void Window::setBaudrate(QString baudrate) {
    unsigned int currentBaudrate = baudrate.split(" ")[0].toUInt();

    if(comBaudrate == currentBaudrate) {
        return;
    }

    comBaudrate = currentBaudrate;
}

void Window::on_sendButton_clicked() {
    if(comPort.getHPort() != nullptr) {
        comPort.writeToCOM(ui->lineToWrite->text().toStdString());
    }
}

void Window::on_sendPackagesButton_clicked() {
    if(comPort.getHPort() != nullptr) {
        comPort.writePackages(ui->lineToWrite->text().toStdString());
    }
}
