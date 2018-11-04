#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"

/**************************************************************************************************/

/* Main Window Constructor & Destructor */

// Main Window Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug("\nApp start.\n");

    qDebug("Setup process start.");

    qDebug("Initializing App Window...");
    ui->setupUi(this);

    qDebug("Setting initial UI elements values...");

    // ComboBox_EOL initialization
    ui->comboBox_EOL->addItems(comboBox_EOL_values);
    ui->comboBox_EOL->setCurrentIndex(eol_default_value);

    // ComboBox_bauds initialization
    ui->comboBox_bauds->addItems(comboBox_bauds_values);
    ui->comboBox_bauds->setCurrentIndex(bauds_default_value);
    ui->comboBox_bauds->setValidator(new QIntValidator(0, 99999999, this) );

    qDebug("Connecting events signals...");

    // Buttons click event setup
    connect(ui->pushButton_open, SIGNAL(released()), this, SLOT(ButtonOpenPressed()));
    connect(ui->pushButton_close, SIGNAL(released()), this, SLOT(ButtonClosePressed()));
    connect(ui->pushButton_clear, SIGNAL(released()), this, SLOT(ButtonClearPressed()));
    connect(ui->pushButton_send, SIGNAL(released()), this, SLOT(ButtonSendPressed()));
    connect(ui->lineEdit_toSend, SIGNAL(returnPressed()), this, SLOT(ButtonSendPressed()));

    // Instantiate SerialPort object and connect received data signal to read event handler
    serial_port = new QSerialPort;
    connect(serial_port, SIGNAL(readyRead()), this, SLOT(SerialReceive()));
    //connect(serial_port, SIGNAL(errorOccurred()), this, SLOT(SerialPortErrorHandler()));

    // Setup and start timer for Serial Ports checks
    qDebug("Initializing Serial Ports check timer...");
    SerialPortsChecks_timer_init();

    qDebug("Setup process end.\n");
}

// Main Window Destructor
MainWindow::~MainWindow()
{
    // If the timer exists and is active, stop and release it
    if(qtimer_serial_ports && qtimer_serial_ports->isActive())
    {
        qtimer_serial_ports->stop();

        delete qtimer_serial_ports;
        qtimer_serial_ports = nullptr;
    }

    // If the SerialPort Object is not null, close and release it
    if(serial_port)
    {
        if((serial_port->isWritable()) || (serial_port->isReadable()))
        {
            serial_port->close();
            qDebug("Port successfully close.");
        }

        delete serial_port;
        serial_port = nullptr;
    }

    // Release UI
    delete ui;

    qDebug("\nApp end.");
}

/**************************************************************************************************/

/* Serial Ports Check */

// Serial Ports check timer initialization and start
void MainWindow::SerialPortsChecks_timer_init(void)
{
    qtimer_serial_ports = new QTimer(this);
    connect(qtimer_serial_ports, SIGNAL(timeout()), this, SLOT(SerialPortsCheck()));
    qtimer_serial_ports->start(500); // Run SerialPortsCheck() each 500ms
}

// Serial Ports check timer fire event handler (this method run each 500ms)
void MainWindow::SerialPortsCheck(void)
{
    QStringList qstrl_actual_ports;

    // Get all availables Serial Ports and add them to a list
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        qstrl_actual_ports.append(info.portName());

    // Add new detected ports to availables ports list
    for(int i = 0; i < qstrl_actual_ports.size(); i++)
    {
        QString port = qstrl_actual_ports[i];
        if (!qstrl_available_serial_ports.contains(port))
        {
            qstrl_available_serial_ports.append(port);
            ui->comboBox_SerialPort->addItem(port);
        }
    }

    // Remove not detected ports from availables ports list
    QList<int> to_rm_indexes;
    for(int i = 0; i < qstrl_available_serial_ports.size(); i++)
    {
        QString port = qstrl_available_serial_ports[i];
        if (!qstrl_actual_ports.contains(port))
            to_rm_indexes.append(i);
    }
    for(const int &i : to_rm_indexes)
    {
        qstrl_available_serial_ports.removeAt(i);
        ui->comboBox_SerialPort->removeItem(i);
    }
}

/**************************************************************************************************/

/* Serial Port Open & Close */

// Button Open pressed event handler
void MainWindow::ButtonOpenPressed(void)
{
    qDebug("Open Button pressed.");
    OpenPort();
}

// Button Close pressed event handler
void MainWindow::ButtonClosePressed(void)
{
    qDebug("Close Button pressed.");
    ClosePort();
}

// Open Serial port if there is any valid selected port and bauds in comboBoxes
void MainWindow::OpenPort(void)
{
    QString selected_port = ui->comboBox_SerialPort->currentText();
    if((selected_port.isNull()) || (selected_port.isEmpty()))
    {
        qDebug("Port is null or empty");
        return;
    }

    QString selected_bauds = ui->comboBox_bauds->currentText();
    if((selected_bauds.isNull()) || (selected_bauds.isEmpty()))
    {
        qDebug("Bauds is null or empty");
        return;
    }

    QByteArray qba_port = selected_port.toUtf8();
    QByteArray qba_selected_bauds = selected_bauds.toUtf8();
    qDebug("Trying to open port %s at %s bauds.", qba_port.data(), qba_selected_bauds.data());
    serial_port->setPortName(selected_port);
    if(serial_port->open(QIODevice::ReadWrite))
    {
        serial_port->setDataBits(QSerialPort::Data8);
        serial_port->setBaudRate(selected_bauds.toInt());
        serial_port->setParity(QSerialPort::NoParity);
        serial_port->setStopBits(QSerialPort::OneStop);
        serial_port->setFlowControl(QSerialPort::NoFlowControl);

        ui->label_status->setStyleSheet("QLabel { color : blue; }");
        ui->label_status->setText("Status: Connected.");

        // Change enable/disable states of UI elements
        ui->pushButton_close->setEnabled(true);
        ui->pushButton_send->setEnabled(true);
        ui->lineEdit_toSend->setEnabled(true);
        ui->pushButton_open->setEnabled(false);
        ui->comboBox_SerialPort->setEnabled(false);

        qDebug("Port successfully open.");
    }
    else
    {
        // An error occurred when opening the port
        QByteArray qba_error = serial_port->errorString().toUtf8();
        qDebug("Error - %s.\n", qba_error.data());

        ui->label_status->setStyleSheet("QLabel { color : red; }");
        ui->label_status->setText("Status: " + serial_port->errorString());
    }
}

// Close serial port if it is open
void MainWindow::ClosePort(void)
{
    // Close the port if it is available
    if((serial_port->isWritable()) || (serial_port->isReadable()))
    {
        serial_port->close();
        qDebug("Port successfully close.");
    }
    else
        qDebug("Port cannot be closed, because is not writable/readable.");

    ui->label_status->setStyleSheet("QLabel { color : black; }");
    ui->label_status->setText("Status: Disconnected.");

    // Change enable/disable states of UI elements
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_send->setEnabled(false);
    ui->lineEdit_toSend->setEnabled(false);
    ui->pushButton_open->setEnabled(true);
    ui->comboBox_SerialPort->setEnabled(true);
}

/**************************************************************************************************/

/* Serial Receive */

// Button clear event handler
void MainWindow::ButtonClearPressed(void)
{
    qDebug("Clear Button pressed.");
    ui->textBrowser_serial->clear();
}

// Serial received data from port
void MainWindow::SerialReceive(void)
{
    //qDebug("Serial data received...");

    // Send the data if the port is available
    if(serial_port->isReadable())
    {
        // Set textbox cursor to bottom
        QTextCursor cursor = ui->textBrowser_serial->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->textBrowser_serial->setTextCursor(cursor);

        // Write the received data to textbox
        ui->textBrowser_serial->insertPlainText(serial_port->readAll());

        // If Autoscroll is checked, scroll to bottom
        if(ui->checkBox_autoScroll->isChecked())
        {
            QScrollBar *vertical_bar = ui->textBrowser_serial->verticalScrollBar();
            vertical_bar->setValue(vertical_bar->maximum());
        }
    }
    else
    {
        // An error occurred when reading from port
        qint64 availables_bytes = serial_port->bytesAvailable();
        qDebug("There is %llu bytes in Rx buffer but them can't be read", availables_bytes);
        QByteArray qba_error = serial_port->errorString().toUtf8();
        qDebug("Error - %s.\n", qba_error.data());
    }
}

/**************************************************************************************************/

/* Serial Sends */

// Button Send pressed event handler
void MainWindow::ButtonSendPressed(void)
{
    qDebug("Send Button pressed.");
    SerialSend();
    ui->lineEdit_toSend->clear();
}

// Serial send data from lineEdit box
void MainWindow::SerialSend(void)
{
    // Check if to send data box is empty
    QString qstr_to_send = ui->lineEdit_toSend->text();
    if((qstr_to_send.isNull()) || (qstr_to_send.isEmpty()))
    {
        qDebug("Data to send box is null or empty");
        return;
    }

    // Append selected end character to data to be send
    qstr_to_send = qstr_to_send + EOL_values[ui->comboBox_EOL->currentIndex()];

    // Send the data if the port is available
    if(serial_port->isWritable())
    {
        // Convert QString to QByteArray to send a char* type
        QByteArray qba_to_send = qstr_to_send.toUtf8();
        serial_port->write(qba_to_send.data());
    }
    else
    {
        // An error occurred when writting to port
        QByteArray qba_error = serial_port->errorString().toUtf8();
        qDebug("Error - %s.\n", qba_error.data());
    }
}

/**************************************************************************************************/

/*void MainWindow::SerialPortErrorHandler(void)
{
    // An error occurred when opening the port
    QByteArray qba_error = serial_port->errorString().toUtf8();
    qDebug("Error - %s.\n", qba_error.data());

    ui->label_status->setStyleSheet("QLabel { color : red; }");
    ui->label_status->setText("Status: " + serial_port->errorString());
}*/
