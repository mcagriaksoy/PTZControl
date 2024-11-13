#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialportworker.h"
#include <QMainWindow>
#include <QThread>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_connectButton_clicked();
    void on_stopButton_clicked();
    void on_saveConfigAsButton_clicked();
    void on_saveButton_clicked();
    void on_aboutButton_clicked();
    void on_checkUpdatesButton_clicked();
    void on_advancedViewButton_clicked();
    void on_keyboard_handler();
    void on_set_limit_handler();
    void on_movement_handler(QString data);
private:
    Ui::MainWindow *ui;
    QThread *workerThread;
    SerialPortWorker *worker;
    void detectSerialPorts();
    void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
