
//
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

// App
#include "mainwindow.h"
#include "ui_mainwindow.h"

QList<QString> StudentNames;
QList<int> StudentScores;

StartTest::StartTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // fill lists
  StudentNames.append("Ig0");
  StudentNames.append("Ig1");
  StudentNames.append("Ig2");

  StudentScores.append(int(0));
  StudentScores.append(int(0));
  StudentScores.append(int(0));

  // table
  QWidget* centralWidget = new QWidget(this);
  this->setCentralWidget(centralWidget);

  QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
  scoreTable_ = new QTableWidget(this);
  scoreTable_->setRowCount(8);
  scoreTable_->setColumnCount(1);
  scoreTable_->setVerticalHeaderLabels(StudentNames);
  //scoreTable_->setHorizontalHeaderLabels(QStringList(1, "Scores"));
  {
    int pos = 0;
    QListIterator<int> i(StudentScores);
    while (i.hasNext()) {
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(i.next()));
        scoreTable_->setItem(pos, 0, item);
        ++pos;
    }
  }

  QPushButton* submit = new QPushButton("Action", this);
  mainLayout->addWidget(scoreTable_);
  mainLayout->addWidget(submit);
}

StartTest::~StartTest()
{
    delete ui;
}
