/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTextBrowser *textBrowser;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLabel *label;
    QCheckBox *checkBox;
    QComboBox *comboBox_EOL;
    QComboBox *comboBox_bauds;
    QLabel *label_2;
    QComboBox *comboBox_SerialPort;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(600, 320);
        MainWindow->setMinimumSize(QSize(600, 320));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMinimumSize(QSize(600, 480));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(20, 90, 571, 181));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 569, 179));
        textBrowser = new QTextBrowser(scrollAreaWidgetContents);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(0, 0, 571, 181));
        scrollArea->setWidget(scrollAreaWidgetContents);
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(20, 60, 471, 21));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(500, 60, 91, 21));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(470, 20, 121, 16));
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(25, 280, 81, 20));
        comboBox_EOL = new QComboBox(centralWidget);
        comboBox_EOL->setObjectName(QStringLiteral("comboBox_EOL"));
        comboBox_EOL->setGeometry(QRect(230, 280, 121, 22));
        comboBox_EOL->setEditable(false);
        comboBox_bauds = new QComboBox(centralWidget);
        comboBox_bauds->setObjectName(QStringLiteral("comboBox_bauds"));
        comboBox_bauds->setGeometry(QRect(360, 280, 121, 22));
        comboBox_bauds->setEditable(true);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 20, 71, 16));
        comboBox_SerialPort = new QComboBox(centralWidget);
        comboBox_SerialPort->setObjectName(QStringLiteral("comboBox_SerialPort"));
        comboBox_SerialPort->setGeometry(QRect(110, 20, 91, 21));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(210, 20, 71, 21));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(290, 20, 71, 21));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(500, 280, 91, 21));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "Send", nullptr));
        label->setText(QApplication::translate("MainWindow", "Status: Disconnected", nullptr));
        checkBox->setText(QApplication::translate("MainWindow", "Autoscroll", nullptr));
        comboBox_EOL->setCurrentText(QString());
        comboBox_bauds->setCurrentText(QApplication::translate("MainWindow", "115200", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Serial Port:", nullptr));
        comboBox_SerialPort->setCurrentText(QString());
        pushButton_2->setText(QApplication::translate("MainWindow", "Open", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "Close", nullptr));
        pushButton_4->setText(QApplication::translate("MainWindow", "Clear", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
