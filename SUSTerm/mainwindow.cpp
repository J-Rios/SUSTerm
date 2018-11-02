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
}

MainWindow::~MainWindow()
{
    delete ui;
}
