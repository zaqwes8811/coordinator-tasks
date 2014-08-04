
//
#include <QPushButton>
#include <QTableWidget>

// App
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QTableWidget* tableWidget = new QTableWidget(this);
  tableWidget->setRowCount(10);
  tableWidget->setColumnCount(5);

  setCentralWidget(tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
