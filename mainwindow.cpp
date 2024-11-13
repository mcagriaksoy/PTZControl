#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    // Connect to the camera
    // Get the data from comboboxes
    QString port = ui->PortComboBox->currentText();
    QString baudrate = ui->BaudrateComboBox->currentText();

    // Set text on label with green color
    ui->statusLabel->setStyleSheet("QLabel { color : green; }");
    ui->statusLabel->setText("Connected to " + port + ":" + baudrate);
}
