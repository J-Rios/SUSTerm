#ifndef CONSTANTS_H
#define CONSTANTS_H

/**************************************************************************************************/

#include <QStringList>

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
    "1200",
    "2400",
    "4800",
    "9600",
    "19200",
    "38400",
    "57600",
    "115200"
};
const int bauds_default_value = 7; // 7 = 115200

const QString ABOUT_TEXT =
    "SUSTerm (Simple Universal Serial Terminal) is a free and open GNU-GPL software which is " \
    "focused in minimalist and be efficiently fast and easy to use.<br/>" \
    "<br/>" \
    "Actual Version: 1.3<br/>" \
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
