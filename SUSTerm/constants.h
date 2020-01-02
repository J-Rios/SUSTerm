#ifndef CONSTANTS_H
#define CONSTANTS_H

/**************************************************************************************************/

#include <QStringList>

/**************************************************************************************************/

/* Debug Enable/Disable and Macro */

#define DEBUG true

#define debug_print(...) do { if(DEBUG) qDebug(__VA_ARGS__); } while (0)

/**************************************************************************************************/

QStringList static comboBox_EOL_values =
{
    "No Line Ending",
    "New Line",
    "Carriage Return",
    "Both NL & CR"
};
QStringList static EOL_values =
{
    "",
    "\n",
    "\r",
    "\r\n"
};
const int eol_default_value = 1; // 1 = New Line

QStringList static comboBox_bauds_values =
{
    "300",
    "1200",
    "2400",
    "4800",
    "9600",
    "19200",
    "38400",
    "57600",
    "74880",
    "115200",
    "230400",
    "250000",
    "500000",
    "1000000",
    "2000000"
};
const int bauds_default_value = 9; // 9 = 115200

const QString ABOUT_TEXT =
    "SUSTerm (Simple Universal Serial Terminal) is a free and open GNU-GPL software which is " \
    "focused in minimalist and be efficiently fast and easy to use.<br/>" \
    "<br/>" \
    "Actual Version: 1.7.0<br/>" \
    "<br/>" \
    "This software was developed by @J-Rios and you can get the code at:<br/>" \
    "&nbsp;&nbsp;&nbsp;&nbsp;" \
    "<a href=\"https://github.com/J-Rios/SUSTerm\">https://github.com/J-Rios/SUSTerm</a><br/>" \
    "<br/>" \
    "Do you like this App? Buy me a coffe:<br/>" \
    "&nbsp;&nbsp;&nbsp;&nbsp;" \
    "Paypal - <a href=\"https://www.paypal.me/josrios\">https://www.paypal.me/josrios</a><br/>" \
    "&nbsp;&nbsp;&nbsp;&nbsp;" \
    "BTC    - 3N9wf3FunR6YNXonquBeWammaBZVzTXTyR" \
    "<p dir=\"rtl\">(: Enjoy this Terminal</p>";

/**************************************************************************************************/

#endif // CONSTANTS_H
