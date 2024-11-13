#ifndef SERIALPORTWORKER_H
#define SERIALPORTWORKER_H

#include <QObject>
#include <QSerialPort>

class SerialPortWorker : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortWorker(const QString &portName, int baudRate, QObject *parent = nullptr);
    ~SerialPortWorker();

    void writeDataToCom(const QByteArray &data);

public slots:
    void start();
    void stop();

signals:
    void errorOccurred(const QString &error);

private:
    QSerialPort *serialPort;
};

#endif // SERIALPORTWORKER_H