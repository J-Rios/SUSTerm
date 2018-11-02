#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**************************************************************************************************/

#include <QMainWindow>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

/**************************************************************************************************/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public slots:
        void SerialPortsCheck(void);

    private:
        Ui::MainWindow *ui;
        QTimer *qtimer_serial_ports;
        QStringList qstrl_available_serial_ports;

        void SerialPortsChecks_timer_init(void);
};

/**************************************************************************************************/

#endif // MAINWINDOW_H
