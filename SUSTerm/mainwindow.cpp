#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"

/**************************************************************************************************/

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

    // Setup and start timer for Serial Ports checks
    qDebug("Initializing Serial Ports check timer...");
    SerialPortsChecks_timer_init();

    // Instantiate SerialPort object
    serial_port = new QSerialPort;

    qDebug("Setup process end.\n");
}

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

void MainWindow::SerialPortsChecks_timer_init(void)
{
    qtimer_serial_ports = new QTimer(this);
    connect(qtimer_serial_ports, SIGNAL(timeout()), this, SLOT(SerialPortsCheck()));
    qtimer_serial_ports->start(500); // Run SerialPortsCheck() each 500ms
}

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

// Open Serial port if there is any valid selected port and bauds in comboBoxes
void MainWindow::ButtonOpenPressed(void)
{
    qDebug("Open Button pressed.");

    QString selected_port = ui->comboBox_SerialPort->currentText();
    if((selected_port.isNull()) || (selected_port.isEmpty()))
    {
        if(selected_port.isNull())
            qDebug("Port is null");
        if(selected_port.isEmpty())
            qDebug("Port is empty");
        return;
    }

    QString selected_bauds = ui->comboBox_bauds->currentText();
    if((selected_bauds.isNull()) || (selected_bauds.isEmpty()))
    {
        if(selected_bauds.isNull())
            qDebug("Bauds is null");
        if(selected_bauds.isEmpty())
            qDebug("Bauds is empty");
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

void MainWindow::ButtonClosePressed(void)
{
    qDebug("Close Button pressed.");

    // Close the port if it is apparently open
    if((serial_port->isWritable()) || (serial_port->isReadable()))
    {
        serial_port->close();
        qDebug("Port successfully close.");
    }
    else
        qDebug("Port is not writable/readable.");

    ui->label_status->setStyleSheet("QLabel { color : black; }");
    ui->label_status->setText("Status: Disconnected.");

    // Change enable/disable states of UI elements
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_send->setEnabled(false);
    ui->lineEdit_toSend->setEnabled(false);
    ui->pushButton_open->setEnabled(true);
    ui->comboBox_SerialPort->setEnabled(true);
}

void MainWindow::SerialPortErrorHandler(void)
{
    if(serial_port->error() == QSerialPort::SerialPortError::DeviceNotFoundError)
        qDebug("Error - Port not found.\n");
    else if (serial_port->error() == QSerialPort::SerialPortError::PermissionError)
        qDebug("Error - No permission for open the port.\n");
    else if (serial_port->error() == QSerialPort::SerialPortError::OpenError)
        qDebug("Error - The port is already open.\n");
    else if (serial_port->error() == QSerialPort::SerialPortError::NotOpenError)
        qDebug("Error - Invalid operation to port that is not open.\n");
    else if (serial_port->error() == QSerialPort::SerialPortError::WriteError)
        qDebug("Error - An I/O error occurred while writing data.\n");
    else if (serial_port->error() == QSerialPort::SerialPortError::ReadError)
        qDebug("Error - An I/O error occurred while reading data..\n");
    else if (serial_port->error() == QSerialPort::SerialPortError::ResourceError)
        qDebug("Error - Port unavailable (unexpected device disconnection).\n");
    else if (serial_port->error() == QSerialPort::SerialPortError::UnsupportedOperationError)
        qDebug("Error - OS unsupported operation with serial port.\n");
    else if (serial_port->error() == QSerialPort::SerialPortError::TimeoutError)
        qDebug("Error - Serial port Timeout.\n");
    else if (serial_port->error() == QSerialPort::SerialPortError::UnknownError)
        qDebug("Error - Serial port unexpected error.\n");
}
