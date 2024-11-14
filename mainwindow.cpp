#include "mainwindow.h"
#include "./ui_mainwindow.h" // todo check why?

#include <QMessageBox>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    workerThread(nullptr)
{
    ui->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);

    // SetIcon
    QIcon icon(":/img/icon.ico");
    this->setWindowIcon(icon);

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::on_stopButton_clicked);
    
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionSave_Config_As, &QAction::triggered, this, &MainWindow::on_saveConfigAsButton_clicked);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::on_saveButton_clicked);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::on_aboutButton_clicked);
    connect(ui->actionCheck_Updates, &QAction::triggered, this, &MainWindow::on_checkUpdatesButton_clicked);
    connect(ui->actionAdvanced_View, &QAction::triggered, this, &MainWindow::on_advancedViewButton_clicked);
    connect(ui->actionHelp, &QAction::triggered, this, [this]() { QMessageBox::information(this, "Help",
     "More information at: github.com/mcagriaksoy"); });
     connect(ui->actionClear_cache, &QAction::triggered, this, [this]() { ui->textBrowser->clear(); });

    // PT Movements
    connect(ui->upButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010008101029"); });
    connect(ui->downButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010010101031"); });
    connect(ui->leftButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010004121027"); });
    connect(ui->rightButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010002132036"); });
    connect(ui->noneButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010000000001"); });
    connect(ui->stopAllButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010000000001"); });
    connect(ui->zoomInButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010020000021"); });
    connect(ui->zoomOutButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010040000041"); });
    connect(ui->focusInButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010080000081"); });
    connect(ui->focusOutButton, &QPushButton::clicked, this, [this]() { on_movement_handler("FF010100000103"); });

    // Checkboxes
    connect(ui->keyboardCheckBox, &QCheckBox::stateChanged, this, &MainWindow::on_keyboard_handler);
    connect(ui->presetEnableCheckBox, &QCheckBox::stateChanged, this, &MainWindow::on_set_limit_handler);

    // Detect serial ports in beginning
    detectSerialPorts();
}

MainWindow::~MainWindow()
{
    // If worker thread is running, stop it
    if (workerThread) {
        workerThread->quit();
        workerThread->wait();
        delete workerThread;
    }
    delete ui;
}

void MainWindow::on_movement_handler(QString data)
{
    // Send data to COM port
    if (workerThread) {
        QByteArray dataBytes = QByteArray::fromHex(data.toLatin1());
        worker->writeDataToCom(dataBytes);
        ui->textBrowser->append("Sent: " + data);
    }
}

void MainWindow::detectSerialPorts()
{
    QStringList portList;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        portList << info.portName() + " - " + info.description();
    }

    if (portList.isEmpty()) {
        QMessageBox::information(this, "Serial Ports", "No serial ports available.");
    }

    // Put the list of ports in a combobox
    ui->PortComboBox->addItems(portList);
}

void MainWindow::on_connectButton_clicked()
{
    // Disable Stop button
    ui->stopButton->setEnabled(true);

    // Get the data from comboboxes
    // Extract port from all text detect COM text and extract port number
    QString port = ui->PortComboBox->currentText().split(" - ")[0];
    QString baudrate = ui->BaudrateComboBox->currentText();

    // Start worker thread to listen to the COM port

    // If there is already a worker thread, delete it
    if (workerThread) {
        workerThread->quit();
        workerThread->wait();
        delete workerThread;
    }

    // Create a new worker thread
    workerThread = new QThread(this);
    worker = new SerialPortWorker(port, baudrate.toInt());
    worker->moveToThread(workerThread);

    // Connect signals and slots
    connect(workerThread, &QThread::started, worker, &SerialPortWorker::start);
    connect(workerThread, &QThread::finished, worker, &SerialPortWorker::deleteLater);

    connect(worker, &SerialPortWorker::errorOccurred, this, [this](const QString &error) {
        QMessageBox::critical(this, "Serial Port Error", error);
    });

    workerThread->start();

    ui->textBrowser->append("Connected to " + port + " with baudrate " + baudrate);

    // Enable the PT buttons
    ui->upButton->setEnabled(true);
    ui->downButton->setEnabled(true);
    ui->leftButton->setEnabled(true);
    ui->rightButton->setEnabled(true);
    ui->noneButton->setEnabled(true);

    // Set text on label with green color
    ui->statusLabel->setStyleSheet("QLabel { color : green; }");
    ui->statusLabel->setText("Connected!");

    // Disable Comboboxes
    ui->PortComboBox->setEnabled(false);
    ui->BaudrateComboBox->setEnabled(false);
}

void MainWindow::on_stopButton_clicked()
{
    // Enable Start button
    ui->connectButton->setEnabled(true);

    // Stop the COM port listener and worker thread
    if (workerThread) {
        workerThread->quit();
        workerThread->wait();
        delete workerThread;
        workerThread = nullptr;
    }
    // Set text on label with red color
    ui->statusLabel->setStyleSheet("QLabel { color : red; }");
    ui->statusLabel->setText("Not Connected Yet");

    // Disable the PT buttons
    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);
    ui->noneButton->setEnabled(false);

    // Enable Comboboxes
    ui->PortComboBox->setEnabled(true);
    ui->BaudrateComboBox->setEnabled(true);
}

void MainWindow::on_saveConfigAsButton_clicked()
{
    // Save the configuration to a file
    // Todo: Implement this feature
}

void MainWindow::on_saveButton_clicked()
{
    // Save the configuration to the same file
    // Todo: Implement this feature
}

void MainWindow::on_aboutButton_clicked()
{
    QMessageBox aboutBox;
    // Create a popup with information about the application
    QString aboutText = "<p>PTZ Control is a simple application to control PTZ cameras.</p>"
                        "<p>GNU LESSER GENERAL PUBLIC LICENSE - GNU LGPL v3.0 </p>"
                        "<p>Version: " + QString(APP_VERSION) + " </p>"
                        "<p>Written by Mehmet Cagri Aksoy - 2024 "
                        "<a href=\"https://github.com/mcagriaksoy/PTZControl\">GitHub Repository</a></p>";
    aboutBox.setTextFormat(Qt::RichText); 
    aboutBox.about(this, "About PTZ Control", aboutText);
}

void MainWindow::on_checkUpdatesButton_clicked()
{
    // Check for updates
    // Get hhtp request to the github page and check the latest version

    // Todo: Implement this feature

    QMessageBox::information(this, "Check Updates", "No updates available. ");
}

void MainWindow::on_advancedViewButton_clicked()
{
    // Show advanced view

    // Todo: Implement this feature

    QMessageBox::information(this, "Advanced View", "This feature is not available yet.");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
        case Qt::Key_Up:
            on_movement_handler("FF010008101029");
            break;
        case Qt::Key_Down:
            on_movement_handler("FF010010101031");
            break;
        case Qt::Key_Left:
            on_movement_handler("FF010004121027");
            break;
        case Qt::Key_Right:
            on_movement_handler("FF010002132036");
            break;
        default:
            on_movement_handler("FF010000000001");
            break;
    }
}

void MainWindow::on_keyboard_handler(void)
{
    // Enable or disable keyboard control
    if (ui->keyboardCheckBox->isChecked()) {
        // Enable keyboard control
        ui->textBrowser->append("Keyboard control enabled.");

        // Connect keyboard events

    } else {
        // Disable keyboard control
        ui->textBrowser->append("Keyboard control disabled.");
    }
}

void MainWindow::on_set_limit_handler(void)
{
    // Enable or disable preset limit control
    if (ui->presetEnableCheckBox->isChecked()) {
        // Enable preset limit control
        ui->textBrowser->append("Preset limit control enabled.");

        // Enable LineEdits
        ui->leftLineEdit->setEnabled(true);
        ui->rightLineEdit->setEnabled(true);

        // Read the values from LineEdits
        int leftLimit = ui->leftLineEdit->text().toInt();
        int rightLimit = ui->rightLineEdit->text().toInt();

        // Check if the values are valid
        if (leftLimit < 0 || leftLimit > 360 || rightLimit < 0 || rightLimit > 360) {
            QMessageBox::warning(this, "Warning", "Please enter a valid degree value between 0 and 360.");
            return;
        }

        // Todo add preset 202,203,204 functionality.

    } else {
        // Disable preset limit control
        ui->textBrowser->append("Preset limit control disabled.");

        // Disable LineEdits
        ui->leftLineEdit->setEnabled(false);
        ui->rightLineEdit->setEnabled(false);
    }
}
