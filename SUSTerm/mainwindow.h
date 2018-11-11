#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**************************************************************************************************/

#include <QMainWindow>
#include <QTextBrowser>
#include <QDialog>
#include <QSize>
#include <QScrollBar>
#include <QKeyEvent>
#include <QDateTime>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "ui_mainwindow.h"
#include "constants.h"

/**************************************************************************************************/

enum terminal_modes { ASCII, HEX, ASCII_HEX };

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
        bool timestamp_on;
        bool timestamp_ms;
        terminal_modes terminal_mode;

        void SerialPortsChecks_timer_init(void);
        void OpenPort(void);
        void ClosePort(void);
        void SerialSend(void);
        void PrintReceivedData(QTextBrowser* textBrowser0, QTextBrowser *textBrowser1,
                               terminal_modes mode);
        QString GetActualSystemTime(void);

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
        void MenuBarExitClick(void);
        void MenuBarTermAsciiClick(void);
        void MenuBarTermHexClick(void);
        void MenuBarTermBothAsciiHexClick(void);
        void MenuBarTimestampClick(void);
        void MenuBarTimestampMsClick(void);
        void MenuBarAboutClick(void);
};

/**************************************************************************************************/

#endif // MAINWINDOW_H
