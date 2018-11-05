#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**************************************************************************************************/

#include <QMainWindow>
#include <QScrollBar>
#include <QKeyEvent>
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

    private:
        Ui::MainWindow *ui;
        QSerialPort *serial_port;
        QTimer *qtimer_serial_ports;
        QStringList qstrl_available_serial_ports;
        QStringList qstrl_send_history;
        int send_history_i;

        void SerialPortsChecks_timer_init(void);
        void OpenPort(void);
        void ClosePort(void);
        void SerialSend(void);

    private slots:
        void SerialPortsCheck(void);
        void ButtonOpenPressed(void);
        void ButtonClosePressed(void);
        void ButtonClearPressed(void);
        void ButtonSendPressed(void);
        void SerialReceive(void);
        void CBoxBaudsChanged(void);
        bool eventFilter(QObject *target, QEvent *event);
        void SerialPortErrorHandler(void);
};

/**************************************************************************************************/

#endif // MAINWINDOW_H
