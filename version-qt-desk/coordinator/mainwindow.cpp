
//
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

// App
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QPushButton *button = new QPushButton(this);
  button->setMinimumWidth(68);
  button->setMinimumHeight(60);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(button);

  QTableWidget* tableWidget = new QTableWidget(this);
  tableWidget->setRowCount(10);
  tableWidget->setColumnCount(5);

  setCentralWidget(tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
