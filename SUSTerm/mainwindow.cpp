#include "mainwindow.h"

/**************************************************************************************************/

/* Main Window Constructor & Destructor */

// Main Window Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    debug_print("\nApp start.\n");

    debug_print("Setup process start.");

    debug_print("Initializing App Window...");
    ui->setupUi(this);

    debug_print("Setting initial UI elements values...");

    // ComboBox_EOL initialization
    ui->comboBox_EOL->addItems(comboBox_EOL_values);
    ui->comboBox_EOL->setCurrentIndex(eol_default_value);

    // ComboBox_bauds initialization
    ui->comboBox_bauds->addItems(comboBox_bauds_values);
    ui->comboBox_bauds->setCurrentIndex(bauds_default_value);
    ui->comboBox_bauds->setValidator(new QIntValidator(0, 99999999, this));

    // TextBrowsers initialization
    QFont font0("Monospace");
    QFont font1("Monospace");
    font0.setStyleHint(QFont::TypeWriter);
    font1.setStyleHint(QFont::TypeWriter);
    font1.setFixedPitch(true);
    ui->textBrowser_serial_0->setFont(font0);
    ui->textBrowser_serial_1->setFont(font1);
    ui->textBrowser_serial_0->clear();
    ui->textBrowser_serial_1->clear();
    ui->textBrowser_serial_1->hide();
    ui->lineEdit_toSend->setFocus();

    debug_print("Connecting events signals...");

    // UI elements user interactions signals event setup
    connect(ui->pushButton_open, SIGNAL(released()), this, SLOT(ButtonOpenPressed()));
    connect(ui->pushButton_close, SIGNAL(released()), this, SLOT(ButtonClosePressed()));
    connect(ui->pushButton_clear, SIGNAL(released()), this, SLOT(ButtonClearPressed()));
    connect(ui->pushButton_send, SIGNAL(released()), this, SLOT(ButtonSendPressed()));
    connect(ui->lineEdit_toSend, SIGNAL(returnPressed()), this, SLOT(ButtonSendPressed()));
    connect(ui->comboBox_bauds, SIGNAL(currentIndexChanged(const QString &)), this,
            SLOT(CBoxBaudsChanged()));
    connect(ui->comboBox_EOL, SIGNAL(currentIndexChanged(const QString &)), this,
            SLOT(CBoxEOLChanged()));
    connect(ui->checkBox_autoScroll, SIGNAL(clicked()), this, SLOT(CheckBoxAutoScrollToggled()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(MenuBarExitClick()));
    connect(ui->actionASCII_Terminal, SIGNAL(triggered()), this, SLOT(MenuBarTermAsciiClick()));
    connect(ui->actionHEX_Terminal, SIGNAL(triggered()), this, SLOT(MenuBarTermHexClick()));
    connect(ui->actionBoth_ASCII_HEX_Terminals, SIGNAL(triggered()), this,
            SLOT(MenuBarTermBothAsciiHexClick()));
    connect(ui->actionEnable_Disable_Timestamp, SIGNAL(triggered()), this,
            SLOT(MenuBarTimestampClick()));
    connect(ui->action_Timestamp_with_ms, SIGNAL(triggered()), this,
            SLOT(MenuBarTimestampMsClick()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(MenuBarAboutClick()));

    // Connect TextBrowsers Scrolls (scroll one of them move the other)
    current_slider_pos_v = 0;
    current_slider_pos_h = 0;
    QScrollBar* scroll_ascii_v = ui->textBrowser_serial_0->verticalScrollBar();
    QScrollBar* scroll_hex_v = ui->textBrowser_serial_1->verticalScrollBar();
    QScrollBar* scroll_ascii_h = ui->textBrowser_serial_0->horizontalScrollBar();
    QScrollBar* scroll_hex_h = ui->textBrowser_serial_1->horizontalScrollBar();
    connect(scroll_ascii_v, &QAbstractSlider::valueChanged,
            [=](int scroll_ascii_slider_pos_v)
            {
                if(scroll_ascii_slider_pos_v != current_slider_pos_v)
                {
                    // Handle autoscroll enable/disable just for more than 100 lines
                    if(scroll_ascii_slider_pos_v > 100)
                    {
                        // Disable autoscroll checkbox if move out of bottom
                        if(scroll_ascii_slider_pos_v < current_slider_pos_v)
                        {
                            if(ui->checkBox_autoScroll->isChecked())
                                ui->checkBox_autoScroll->setChecked(false);
                        }

                        // Enable autoscroll checkbox if move to bottom
                        if(scroll_ascii_slider_pos_v >= scroll_ascii_v->maximum())
                        {
                            if(!ui->checkBox_autoScroll->isChecked())
                                ui->checkBox_autoScroll->setChecked(true);
                        }
                    }

                    scroll_hex_v->setValue(scroll_ascii_slider_pos_v);
                    current_slider_pos_v = scroll_ascii_slider_pos_v;
                }
            });
    connect(scroll_hex_v, &QAbstractSlider::valueChanged,
            [=](int scroll_hex_slider_pos_v)
            {
                if(scroll_hex_slider_pos_v != current_slider_pos_v)
                {
                    scroll_ascii_v->setValue(scroll_hex_slider_pos_v);
                    current_slider_pos_v = scroll_hex_slider_pos_v;
                }
            });
    connect(scroll_ascii_h, &QAbstractSlider::valueChanged,
            [=](int scroll_ascii_slider_pos_h)
            {
                if(scroll_ascii_slider_pos_h != current_slider_pos_h)
                {
                    scroll_hex_h->setValue(scroll_ascii_slider_pos_h);
                    current_slider_pos_h = scroll_ascii_slider_pos_h;
                }
            });
    connect(scroll_hex_h, &QAbstractSlider::valueChanged,
            [=](int scroll_hex_slider_pos_h)
            {
                if(scroll_hex_slider_pos_h != current_slider_pos_h)
                {
                    scroll_ascii_h->setValue(scroll_hex_slider_pos_h);
                    current_slider_pos_h = scroll_hex_slider_pos_h;
                }
            });

    // Set initial configs to default values
    timestamp_on = false;
    timestamp_ms = false;
    terminal_mode = ASCII;

    // Instantiate SerialPort object and connect received data and error signal to event handlers
    serial_port = new QSerialPort;
    connect(serial_port, SIGNAL(readyRead()), this, SLOT(SerialReceive()));
    connect(serial_port, QOverload<QSerialPort::SerialPortError>::of(&QSerialPort::errorOccurred),
            this, [=](){SerialPortErrorHandler();});

    // Setup and start timer for Serial Ports checks
    debug_print("Initializing Serial Ports check timer...");
    SerialPortsChecks_timer_init();

    // Add an event filter to catch keyboard signals with an eventFilter() handler
    installEventFilter(this);

    // Inicial send history index to -1
    send_history_i = -1;

    debug_print("Setup process end.\n");
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
            debug_print("Port successfully close.");
        }

        delete serial_port;
        serial_port = nullptr;
    }

    // Release UI
    delete ui;

    debug_print("\nApp end.");
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
    debug_print("Open Button pressed.");
    OpenPort();
}

// Button Close pressed event handler
void MainWindow::ButtonClosePressed(void)
{
    debug_print("Close Button pressed.");
    ClosePort();
}

// Open Serial port if there is any valid selected port and bauds in comboBoxes
void MainWindow::OpenPort(void)
{
    QString selected_port = ui->comboBox_SerialPort->currentText();
    if((selected_port.isNull()) || (selected_port.isEmpty()))
    {
        debug_print("Port is null or empty");
        return;
    }

    QString selected_bauds = ui->comboBox_bauds->currentText();
    if((selected_bauds.isNull()) || (selected_bauds.isEmpty()))
    {
        debug_print("Bauds is null or empty");
        return;
    }

    QByteArray qba_port = selected_port.toUtf8();
    QByteArray qba_selected_bauds = selected_bauds.toUtf8();
    debug_print("Trying to open port %s at %s bauds.", qba_port.data(), qba_selected_bauds.data());
    serial_port->setPortName(selected_port);
    if(serial_port->open(QIODevice::ReadWrite))
    {
        serial_port->setDataBits(QSerialPort::Data8);
        serial_port->setBaudRate(selected_bauds.toInt());
        serial_port->setParity(QSerialPort::NoParity);
        serial_port->setStopBits(QSerialPort::OneStop);
        serial_port->setFlowControl(QSerialPort::NoFlowControl);

        // Clear possible buffered data
        serial_port->clear();

        ui->label_status->setStyleSheet("QLabel { color : blue; }");
        ui->label_status->setText("Status: Connected.");

        // Change enable/disable states of UI elements
        ui->pushButton_close->setEnabled(true);
        ui->pushButton_send->setEnabled(true);
        ui->lineEdit_toSend->setEnabled(true);
        ui->pushButton_open->setEnabled(false);
        ui->comboBox_SerialPort->setEnabled(false);

        ui->lineEdit_toSend->setFocus();

        debug_print("Port successfully open.");
    }
}

// Close serial port if it is open
void MainWindow::ClosePort(void)
{
    // Close the port if it is available
    if((serial_port->isWritable()) || (serial_port->isReadable()))
    {
        serial_port->close();
        debug_print("Port successfully close.");

        ui->label_status->setStyleSheet("QLabel { color : black; }");
        ui->label_status->setText("Status: Disconnected.");
    }

    // Change enable/disable states of UI elements
    ui->pushButton_close->setEnabled(false);
    ui->pushButton_send->setEnabled(false);
    ui->lineEdit_toSend->setEnabled(false);
    ui->pushButton_open->setEnabled(true);
    ui->comboBox_SerialPort->setEnabled(true);
}

/**************************************************************************************************/

/* Bauds, Send EOL bytes and Auto-Scroll changes */

// ComboBox Bauds change event handler
void MainWindow::CBoxBaudsChanged(void)
{
    debug_print("Bauds changed.");

    // Close the port if it is already open
    if((serial_port->isWritable()) || (serial_port->isReadable()))
    {
        debug_print("Reconnecting to port using new baudrate.");
        ClosePort();
        OpenPort();
    }
}

// ComboBox EOL change event handler
void MainWindow::CBoxEOLChanged(void)
{
    debug_print("Send EOL changed.");

    ui->lineEdit_toSend->setFocus();
}

// CheckBox AutoScroll toggled event handler
void MainWindow::CheckBoxAutoScrollToggled(void)
{
    debug_print("Auto-Scroll CheckBox changed.");
}

/**************************************************************************************************/

/* Serial Receive */

// Button clear event handler
void MainWindow::ButtonClearPressed(void)
{
    debug_print("Clear Button pressed.");
    ui->textBrowser_serial_0->clear();
    ui->textBrowser_serial_1->clear();
    ui->lineEdit_toSend->setFocus();
    ui->checkBox_autoScroll->setChecked(true);
}

// Serial received data from port
void MainWindow::SerialReceive(void)
{
    // Send the data if the port is available
    if(serial_port->isReadable())
        PrintReceivedData(ui->textBrowser_serial_0, ui->textBrowser_serial_1, terminal_mode);
}

// Print the received serial data
void MainWindow::PrintReceivedData(QTextBrowser* textBrowser0, QTextBrowser *textBrowser1,
                                   terminal_modes mode)
{
    static uint32_t hex_line_num_chars = 0;

    // First thing to do is get actual time
    QByteArray qba_time;
    if(timestamp_on)
        qba_time = QString("[" + GetActualSystemTime() + "] ").toUtf8();

    // EOL in QByteArray
    QByteArray qba_eol = QString("\n").toUtf8();

    // If terminal mode is ASCII or HEX (Single TextBrowser)
    if((mode == ASCII) || (mode == HEX))
    {
        // Get the received data and split it by lines
        QByteArray serial_data = serial_port->readAll();
        QList<QByteArray> lines = serial_data.split('\n');
        //debug_print("Received data: %s", serial_data.toHex().data());

        // Get data last character
        char data_last_char = serial_data[serial_data.size()-1];
        static bool last_line_was_eol = false;

        if(mode == ASCII)
        {
            // For each line of received data
            int num_lines = lines.length();
            for(int i = 0; i < num_lines; i++)
            {
                // Get the ASCII data line
                QByteArray to_print_ascii = lines[i];

                // Remove carriage return characters
                QString qstr_to_print_ascii(to_print_ascii);
                qstr_to_print_ascii = qstr_to_print_ascii.remove(QChar('\r'));

                // Add time to data if it is not the first line
                if(i != 0)
                    qstr_to_print_ascii = qstr_to_print_ascii.prepend(qba_time);
                else
                {
                    // Add time to data if the last written line has an end of line
                    if(last_line_was_eol)
                        qstr_to_print_ascii = qstr_to_print_ascii.prepend(qba_time);
                    else
                        last_line_was_eol = false;
                }

                // Recover lost EOL due to split if it is not the last line
                bool ignore_last_split_line = false;
                if(i < num_lines-1)
                    qstr_to_print_ascii = qstr_to_print_ascii.append(qba_eol);
                else
                {
                    // Data last character is an EOL
                    if(data_last_char == '\n')
                    {
                        ignore_last_split_line = true;
                        last_line_was_eol = true;
                    }
                    else
                        last_line_was_eol = false;
                }

                // Ignore print this line
                if(!ignore_last_split_line)
                {
                    // Convert all non-basic printable ASCII characters to ? symbol (ASCII 63)
                    ushort ascii_num;
                    for(int i = 0; i < qstr_to_print_ascii.length(); i++)
                    {
                        ascii_num = qstr_to_print_ascii[i].unicode();
                        if((ascii_num < 32) || (ascii_num > 126))
                        {
                            // If actual char is not a tabulation ('\t') and not an end of line ('\n')
                            if((ascii_num != 9) && (ascii_num != 10))
                                qstr_to_print_ascii[i] = (QChar)(63);
                        }
                    }

                    // Get original cursor and scroll position
                    QTextCursor original_cursor_pos = textBrowser0->textCursor();
                    QScrollBar *vertical_bar = textBrowser0->verticalScrollBar();
                    QScrollBar *horizontal_bar = textBrowser0->horizontalScrollBar();
                    int original_scroll_pos_v = vertical_bar->value();
                    int original_scroll_pos_h = horizontal_bar->value();

                    // Set textbox cursor to bottom
                    QTextCursor new_cursor = original_cursor_pos;
                    new_cursor.movePosition(QTextCursor::End);
                    textBrowser0->setTextCursor(new_cursor);

                    // Write data line to textbox
                    textBrowser0->insertPlainText(qstr_to_print_ascii);

                    // If Autoscroll is checked, scroll to bottom
                    if(ui->checkBox_autoScroll->isChecked())
                    {
                        vertical_bar->triggerAction(QAbstractSlider::SliderToMaximum);
                        vertical_bar->setValue(vertical_bar->maximum());
                    }
                    else
                    {
                        // Return to previous cursor and scrolls position
                        textBrowser0->setTextCursor(original_cursor_pos);
                        vertical_bar->setValue(original_scroll_pos_v);
                        horizontal_bar->setValue(original_scroll_pos_h);
                    }
                }
            }
        }
        else
        {
            // For each line of received data
            int num_lines = lines.length();
            for(int i = 0; i < num_lines; i++)
            {
                // Get the HEX data line and format it to string
                QByteArray to_print_hex = lines[i].toHex();
                to_print_hex = to_print_hex.toUpper();

                // If in this line we have yet 2 bytes printed, add a space before new data
                if(hex_line_num_chars % 2)
                    to_print_hex.prepend(' ');

                if(to_print_hex.length() > 2)
                {
                    // Add spaces between pair of bytes of hex data (convert "A2345F" to "A2 34 5F")
                    for(int ii = 2; ii < to_print_hex.length()-1; ii = ii + 3)
                        to_print_hex.insert(ii, " ");
                   to_print_hex.append(' ');
                }
                else if (to_print_hex.length() == 2)
                {
                    // If there is just one byte and is an EOL, add a space
                    if(lines[i][0] != '\n')
                        to_print_hex.append(' ');
                }

                // Add time to data if it is not the first line
                if(i != 0)
                    to_print_hex = to_print_hex.prepend(qba_time);
                else
                {
                    // Add time to data if the last written line has an end of line
                    if(last_line_was_eol)
                        to_print_hex = to_print_hex.prepend(qba_time);
                    else
                        last_line_was_eol = false;
                }

                // Recover lost EOL due to split if it is not the last line
                bool ignore_last_split_line = false;
                if(i < num_lines-1)
                {
                    to_print_hex = to_print_hex.append("0A" + qba_eol);
                    hex_line_num_chars = 0;
                }
                else
                {
                    // Data last character is an EOL
                    if(data_last_char == '\n')
                    {
                        ignore_last_split_line = true;
                        last_line_was_eol = true;
                    }
                    else
                        last_line_was_eol = false;
                }

                // Ignore print this line
                if(!ignore_last_split_line)
                {
                    // Get original cursor and scroll position
                    QTextCursor original_cursor_pos = textBrowser0->textCursor();
                    QScrollBar *vertical_bar = textBrowser0->verticalScrollBar();
                    QScrollBar *horizontal_bar = textBrowser0->horizontalScrollBar();
                    int original_scroll_pos_v = vertical_bar->value();
                    int original_scroll_pos_h = horizontal_bar->value();

                    // Set textbox cursor to bottom
                    QTextCursor new_cursor = original_cursor_pos;
                    new_cursor.movePosition(QTextCursor::End);
                    textBrowser0->setTextCursor(new_cursor);

                    // Write data line to textbox
                    textBrowser0->insertPlainText(to_print_hex);

                    // If Autoscroll is checked, scroll to bottom
                    if(ui->checkBox_autoScroll->isChecked())
                    {
                        vertical_bar->triggerAction(QAbstractSlider::SliderToMaximum);
                        vertical_bar->setValue(vertical_bar->maximum());
                    }
                    else
                    {
                        // Return to previous cursor and scroll position
                        textBrowser0->setTextCursor(original_cursor_pos);
                        vertical_bar->setValue(original_scroll_pos_v);
                        horizontal_bar->setValue(original_scroll_pos_h);
                    }
                }
            }
        }
    }
    else if (mode == ASCII_HEX)
    {
        // Get the received data and split it by lines
        QByteArray serial_data = serial_port->readAll();
        QList<QByteArray> lines = serial_data.split('\n');

        // Get data last character
        char data_last_char = serial_data[serial_data.size()-1];
        static bool last_line_was_eol = true;

        // For each line of received data
        int num_lines = lines.length();
        for(int i = 0; i < num_lines; i++)
        {
            // Get the ASCII data line
            QByteArray to_print_ascii = lines[i];

            // Remove carriage return characters
            QString qstr_to_print_ascii(to_print_ascii);
            qstr_to_print_ascii = qstr_to_print_ascii.remove(QChar('\r'));

            // Get the HEX data line and format it to string
            QByteArray to_print_hex = lines[i].toHex();
            to_print_hex = to_print_hex.toUpper();
            if(to_print_hex.length() > 2)
            {
                // Add spaces between pair of bytes of hex data (convert "A2345F" to "A2 34 5F")
                for(int ii = 2; ii < to_print_hex.length()-1; ii = ii + 3)
                    to_print_hex.insert(ii, " ");
                to_print_hex.append(' ');
             }
             else if (to_print_hex.length() == 2)
             {
                 // If there is just one byte and is an EOL, add a space
                 if(lines[i][0] != '\n')
                     to_print_hex.append(' ');
             }

            // Add time to data if it is not the first line
            if(i != 0)
            {
                qstr_to_print_ascii = qstr_to_print_ascii.prepend(qba_time);
                to_print_hex = to_print_hex.prepend(qba_time);
            }
            else
            {
                // Add time to data if the last written line has an end of line
                if(last_line_was_eol)
                {
                    qstr_to_print_ascii = qstr_to_print_ascii.prepend(qba_time);
                    to_print_hex = to_print_hex.prepend(qba_time);
                }
                else
                    last_line_was_eol = false;
            }

            bool ignore_last_split_line = false;
            // Recover lost EOL due to split if it is not the last line
            if(i < num_lines-1)
            {
                qstr_to_print_ascii = qstr_to_print_ascii.append(qba_eol);
                to_print_hex = to_print_hex.append("0A" + qba_eol);
            }
            else
            {
                // Data last character is an EOL
                if(data_last_char == '\n')
                {
                    ignore_last_split_line = true;
                    last_line_was_eol = true;
                }
                else
                    last_line_was_eol = false;
            }

            // Ignore print this line
            if(!ignore_last_split_line)
            {
                // Convert all non-basic printable ASCII characters to ? symbol (ASCII 63)
                ushort ascii_num;
                for(int i = 0; i < qstr_to_print_ascii.length(); i++)
                {
                    ascii_num = qstr_to_print_ascii[i].unicode();
                    if((ascii_num < 32) || (ascii_num > 126))
                    {
                        // If actual char is not a tabulation ('\t') and not an end of line ('\n')
                        if((ascii_num != 9) && (ascii_num != 10))
                            qstr_to_print_ascii[i] = QChar(63);
                    }
                }

                // Get original cursor and scroll position
                QTextCursor original_cursor_pos_ascii = textBrowser0->textCursor();
                QTextCursor original_cursor_pos_hex = textBrowser1->textCursor();
                QScrollBar *vertical_bar_ascii = textBrowser0->verticalScrollBar();
                QScrollBar *horizontal_bar_ascii = textBrowser0->horizontalScrollBar();
                QScrollBar *vertical_bar_hex = textBrowser1->verticalScrollBar();
                QScrollBar *horizontal_bar_hex = textBrowser1->horizontalScrollBar();
                int original_scroll_pos_ascii_v = vertical_bar_ascii->value();
                int original_scroll_pos_ascii_h = horizontal_bar_ascii->value();
                int original_scroll_pos_hex_v = vertical_bar_hex->value();
                int original_scroll_pos_hex_h = horizontal_bar_hex->value();

                // Set textbox cursor to bottom
                QTextCursor new_cursor_ascii = original_cursor_pos_ascii;
                QTextCursor new_cursor_hex = original_cursor_pos_hex;
                new_cursor_ascii.movePosition(QTextCursor::End);
                new_cursor_hex.movePosition(QTextCursor::End);
                textBrowser0->setTextCursor(new_cursor_ascii);
                textBrowser1->setTextCursor(new_cursor_hex);

                // Write data line to ASCII and HEX textboxes
                textBrowser0->insertPlainText(qstr_to_print_ascii);
                textBrowser1->insertPlainText(to_print_hex);

                // If Autoscroll is checked, scroll to bottom
                if(ui->checkBox_autoScroll->isChecked())
                {
                    vertical_bar_ascii->triggerAction(QAbstractSlider::SliderToMaximum);
                    vertical_bar_hex->triggerAction(QAbstractSlider::SliderToMaximum);
                    vertical_bar_ascii->setValue(vertical_bar_ascii->maximum());
                    vertical_bar_hex->setValue(vertical_bar_hex->maximum());
                    horizontal_bar_ascii->setValue(original_scroll_pos_ascii_h);
                    horizontal_bar_hex->setValue(original_scroll_pos_hex_h);
                }
                else
                {
                    // Return to previous cursor and scroll position
                    textBrowser0->setTextCursor(original_cursor_pos_ascii);
                    textBrowser1->setTextCursor(original_cursor_pos_hex);
                    vertical_bar_ascii->setValue(original_scroll_pos_ascii_v);
                    vertical_bar_hex->setValue(original_scroll_pos_hex_v);
                    horizontal_bar_ascii->setValue(original_scroll_pos_ascii_h);
                    horizontal_bar_hex->setValue(original_scroll_pos_hex_h);
                }
            }
        }
    }
}

// Get an string of actual system time
QString MainWindow::GetActualSystemTime(void)
{
    QString time;

    // Get local date
    QDateTime date_local(QDateTime::currentDateTime());

    // This is how to get UTC instead local
    /*QDateTime date_UTC(QDateTime::currentDateTime());
    date_UTC.setTimeSpec(Qt::UTC);*/

    time = date_local.time().toString();

    if(timestamp_ms)
    {
        int i_ms = date_local.time().msec();
        QString str_ms;

        if(i_ms < 10)
            str_ms = "00" + QString::number(i_ms);
        else if (i_ms < 100)
            str_ms = "0" + QString::number(i_ms);
        else
            str_ms = QString::number(i_ms);

        time = time + ":" + str_ms;
    }

    return time;
}

/**************************************************************************************************/

/* Serial Sends */

// Button Send pressed event handler
void MainWindow::ButtonSendPressed(void)
{
    debug_print("Send Button pressed.");
    SerialSend();
    ui->lineEdit_toSend->clear();
    ui->lineEdit_toSend->setFocus();
}

// Serial send data from lineEdit box
void MainWindow::SerialSend(void)
{
    QScrollBar* vertical_bar = ui->textBrowser_serial_0->verticalScrollBar();

    // Check if to send data box is empty
    QString qstr_to_send = ui->lineEdit_toSend->text();
    if(qstr_to_send.isNull())
    {
        debug_print("Data to send box is null.");
        return;
    }
    if(qstr_to_send.isEmpty())
    {
        qstr_to_send = EOL_values[ui->comboBox_EOL->currentIndex()];

        if(qstr_to_send != "")
        {
            // Send the data if the port is available
            if(serial_port->isWritable())
            {
                // Convert QString to QByteArray to send a char* type
                QByteArray qba_to_send = qstr_to_send.toUtf8();
                serial_port->write(qba_to_send.data());
            }

            ui->lineEdit_toSend->setFocus();

            // Check autoscroll and go to bottom
            ui->checkBox_autoScroll->setChecked(true);
            vertical_bar->triggerAction(QAbstractSlider::SliderToMaximum);
            vertical_bar->setValue(vertical_bar->maximum());

            return;
        }
    }

    // Add to send data to history list if it is not there yet
    if(!qstrl_send_history.isEmpty())
    {
        if(qstr_to_send != qstrl_send_history[0])
            qstrl_send_history.prepend(qstr_to_send);
    }
    else
        qstrl_send_history.prepend(qstr_to_send);
    send_history_i = -1;

    // Append selected end character to data to be send
    qstr_to_send = qstr_to_send + EOL_values[ui->comboBox_EOL->currentIndex()];

    // Send the data if the port is available
    if(serial_port->isWritable())
    {
        // Convert QString to QByteArray to send a char* type
        QByteArray qba_to_send = qstr_to_send.toUtf8();
        serial_port->write(qba_to_send.data());
    }

    // Check autoscroll and go to bottom
    ui->checkBox_autoScroll->setChecked(true);
    vertical_bar->triggerAction(QAbstractSlider::SliderToMaximum);
    vertical_bar->setValue(vertical_bar->maximum());

    ui->lineEdit_toSend->setFocus();
}

/**************************************************************************************************/

/* History Change Of To Send Data */

// Keyboard keys pressed event handler
bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    // If Event is a keyboard key press
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        // If Key pressed is Up arrow, the focus is set to send box and history is not empty
        if(keyEvent->key() == Qt::Key_Up)
        {
            if(ui->lineEdit_toSend->hasFocus())
            {
                if(!qstrl_send_history.isEmpty() && (send_history_i >= -1))
                {
                    if(send_history_i < qstrl_send_history.size() - 1)
                    {
                        send_history_i = send_history_i + 1;
                        ui->lineEdit_toSend->setText(qstrl_send_history[send_history_i]);
                    }
                }
            }
        }

        // If Key pressed is Down arrow, the focus is set to send box and history is not empty
        else if(keyEvent->key() == Qt::Key_Down)
        {
            if(ui->lineEdit_toSend->hasFocus())
            {
                if(!qstrl_send_history.isEmpty())
                {
                    if(send_history_i > -1)
                        send_history_i = send_history_i - 1;

                    if(send_history_i > -1)
                        ui->lineEdit_toSend->setText(qstrl_send_history[send_history_i]);
                    else
                        ui->lineEdit_toSend->clear();
                }
            }
        }

        // Check to set focus in send data box
        else
        {
            // Ignore if focus is currently in
            if(ui->lineEdit_toSend->hasFocus())
                return QObject::eventFilter(target, event);

            // Ignore if send box is not enabled
            if(!ui->lineEdit_toSend->isEnabled())
                return QObject::eventFilter(target, event);

            // Ignore if it is a modifier key event or a non-shift modifier key cobination
            // press (i.e. CTRL+KEY)
            if((!keyEvent->modifiers()) || (keyEvent->modifiers() & Qt::ShiftModifier))
            {
                // Add key text if it is not the ENTER key
                if((keyEvent->key() != Qt::Key_Enter) && (keyEvent->key() != Qt::Key_Return))
                {
                    QString text = ui->lineEdit_toSend->text() + keyEvent->text();
                    ui->lineEdit_toSend->setText(text);
                }

                // Set focus
                ui->lineEdit_toSend->setFocus();
            }
        }
    }

    return QObject::eventFilter(target, event);
}

/**************************************************************************************************/

/* Serial Port Error Signal Handler */

// An error occurred in the port
void MainWindow::SerialPortErrorHandler(void)
{
    if(serial_port->error() != QSerialPort::NoError)
    {
        QByteArray qba_error = serial_port->errorString().toUtf8();
        debug_print("Error - %s\n", qba_error.data());

        ui->label_status->setStyleSheet("QLabel { color : red; }");
        ui->label_status->setText("Status: " + serial_port->errorString());

        // Force close
        ClosePort();
    }
}

/**************************************************************************************************/

/* Menu Bar Functions */

void MainWindow::MenuBarExitClick(void)
{
    QApplication::quit();
}

void MainWindow::MenuBarTermAsciiClick(void)
{
    terminal_mode = ASCII;
    ui->actionASCII_Terminal->setEnabled(false);

    ui->actionHEX_Terminal->setEnabled(true);
    ui->actionBoth_ASCII_HEX_Terminals->setEnabled(true);
    ui->actionHEX_Terminal->setChecked(false);
    ui->actionBoth_ASCII_HEX_Terminals->setChecked(false);

    // Hide the second textBrowser
    ui->textBrowser_serial_1->hide();
}

void MainWindow::MenuBarTermHexClick(void)
{
    terminal_mode = HEX;
    ui->actionHEX_Terminal->setEnabled(false);

    ui->actionASCII_Terminal->setEnabled(true);
    ui->actionBoth_ASCII_HEX_Terminals->setEnabled(true);
    ui->actionASCII_Terminal->setChecked(false);
    ui->actionBoth_ASCII_HEX_Terminals->setChecked(false);

    // Hide the second textBrowser
    ui->textBrowser_serial_1->hide();
}

void MainWindow::MenuBarTermBothAsciiHexClick(void)
{
    terminal_mode = ASCII_HEX;
    ui->actionBoth_ASCII_HEX_Terminals->setEnabled(false);

    ui->actionASCII_Terminal->setEnabled(true);
    ui->actionHEX_Terminal->setEnabled(true);
    ui->actionASCII_Terminal->setChecked(false);
    ui->actionHEX_Terminal->setChecked(false);

    // Show the second textBrowser and set cursor and scroll position same as first textBrowser
    QString eols;
    size_t cursor_pos = static_cast<size_t>(ui->textBrowser_serial_0->document()->lineCount()-1);
    for(size_t i = 0; i < cursor_pos; i++)
        eols.append("\n");
    ui->textBrowser_serial_1->setPlainText(eols);
    QTextCursor cursor = ui->textBrowser_serial_1->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser_serial_1->setTextCursor(cursor);
    int scroll_pos = ui->textBrowser_serial_0->verticalScrollBar()->value();
    QScrollBar *vertical_bar = ui->textBrowser_serial_1->verticalScrollBar();
    if(ui->checkBox_autoScroll->isChecked())
    {
        vertical_bar->triggerAction(QAbstractSlider::SliderToMaximum);
        vertical_bar->setValue(vertical_bar->maximum());
    }
    else
        vertical_bar->setValue(scroll_pos);
    ui->textBrowser_serial_1->show();
}

void MainWindow::MenuBarTimestampClick(void)
{
    timestamp_on = !timestamp_on;

    if(timestamp_on)
        ui->action_Timestamp_with_ms->setEnabled(true);
    else
    {
        ui->action_Timestamp_with_ms->setChecked(false);
        ui->action_Timestamp_with_ms->setEnabled(false);
        timestamp_ms = false;
    }
}

void MainWindow::MenuBarTimestampMsClick(void)
{
    timestamp_ms = !timestamp_ms;
}

void MainWindow::MenuBarAboutClick(void)
{
    // Create a Dialog who is child of mainwindow
    QDialog *dialog = new QDialog(this, Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    dialog->setWindowTitle("About SUSTerm");
    dialog->setMinimumSize(QSize(480, 280));
    dialog->setMaximumSize(QSize(480, 280));

    // Add Dialog about text
    QLabel* label = new QLabel(dialog);
    label->setTextFormat(Qt::RichText);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label->setOpenExternalLinks(true);
    label->setText(ABOUT_TEXT);
    QHBoxLayout* layout = new QHBoxLayout(dialog);
    layout->setContentsMargins(30,20,30,30);
    layout->addWidget(label);
    dialog->setLayout(layout);

    // Execute the Dialog (show() instead dont block parent user interactions)
    dialog->exec();
}
