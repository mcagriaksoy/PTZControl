#include "serialportworker.h"
#include <QSerialPortInfo>
#include <QDebug>

SerialPortWorker::SerialPortWorker(const QString &portName, int baudRate, QObject *parent)
    : QObject(parent), serialPort(new QSerialPort(this))
{
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);

    connect(serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        if (error != QSerialPort::NoError) {
            emit errorOccurred(serialPort->errorString());
        }
    });
}

SerialPortWorker::~SerialPortWorker()
{
    stop();
}

void SerialPortWorker::start()
{
    if (!serialPort->open(QIODevice::ReadWrite)) {
        emit errorOccurred(serialPort->errorString());
    }
}

void SerialPortWorker::stop()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

void SerialPortWorker::writeDataToCom(const QByteArray &data)
{
    if (serialPort->isOpen()) {
        qint64 bytesWritten = serialPort->write(data);
        if (bytesWritten == -1) {
            emit errorOccurred(serialPort->errorString());
        }
    } else {
        emit errorOccurred("Serial port is not open");
    }
}