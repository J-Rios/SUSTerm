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
const int eol_default_value = 1; // 1 = New Line

QStringList static comboBox_bauds_values =
{
    "110",
    "300",
    "600",
    "1200",
    "2400",
    "4800",
    "9600",
    "14400",
    "19200",
    "38400",
    "57600",
    "115200",
    "128000",
    "256000"
};
const int bauds_default_value = 11; // 11 = 115200

/**************************************************************************************************/

#endif // CONSTANTS_H
