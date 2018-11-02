#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"

/**************************************************************************************************/

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ComboBox_EOL initialization
    ui->comboBox_EOL->addItems(comboBox_EOL_values);
    ui->comboBox_EOL->setCurrentIndex(eol_default_value);

    // ComboBox_bauds initialization
    ui->comboBox_bauds->addItems(comboBox_bauds_values);
    ui->comboBox_bauds->setCurrentIndex(bauds_default_value);

    // Setup and start timer for Serial Ports checks
    SerialPortsChecks_timer_init();
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


    delete ui;
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
