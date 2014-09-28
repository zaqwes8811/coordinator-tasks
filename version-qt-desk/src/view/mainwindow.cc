#include "top/config.h"

// App
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "test_help_data.h"

//
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <boost/bind.hpp>
#include <adobe/algorithm/for_each.hpp>

using app_core::AppCore;

using boost::ref;
using boost::bind;

static QList<QString> s_student_names_;
static QList<int> s_student_scores_;

StartTest::StartTest(app_core::AppCore* const app_ptr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    app_ptr_(app_ptr)
{
  ui->setupUi(this);

  // fill lists
  s_student_names_.append("Ig0");
  s_student_names_.append("Ig1");
  s_student_names_.append("Ig2");

  s_student_scores_.append(int(0));
  s_student_scores_.append(int(0));
  s_student_scores_.append(int(0));

  // table
  QWidget* centralWidget = new QWidget(this);
  this->setCentralWidget(centralWidget);

  QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
  scoreTable_ = new QTableWidget(this);
  scoreTable_->setRowCount(8);
  scoreTable_->setColumnCount(1);
  scoreTable_->setVerticalHeaderLabels(s_student_names_);
  //scoreTable_->setHorizontalHeaderLabels(QStringList(1, "Scores"));
  {
    int pos = 0;
    QListIterator<int> i(s_student_scores_);
    while (i.hasNext()) {
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(i.next()));
        scoreTable_->setItem(pos, 0, item);
        ++pos;
    }
  }

  QPushButton* submit = new QPushButton("Add records", this);
  connect(submit, SIGNAL(clicked(bool)), this, SLOT(slotAddRecords(bool)));
  mainLayout->addWidget(scoreTable_);
  mainLayout->addWidget(submit);
}

StartTest::~StartTest()
{
    delete ui;
}

void StartTest::slotAddRecords(bool checked) {
    domain::TasksMirror mirror(test_help_data::build_fake_model());
    adobe::for_each(mirror, bind(&AppCore::append, ref(*app_ptr_), _1));
}

void StartTest::updateAction() {
 int i;
 i = 0;
}
