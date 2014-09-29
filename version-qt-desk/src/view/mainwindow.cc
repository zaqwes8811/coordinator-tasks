// selection problem:
//  http://stackoverflow.com/questions/11816272/qtablewidget-cellactivated-signal-not-working
//
// How check edited state:
//   http://www.qtcentre.org/threads/7976-How-to-know-when-a-cell-in-a-QTableWidget-has-been-edited
//   http://qt-project.org/forums/viewthread/31372

#include "top/config.h"

// App
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "test_help_data.h"

//
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <boost/bind.hpp>
#include <adobe/algorithm/for_each.hpp>

#include <string>

using app_core::AppCore;
using domain::TasksMirror;  // not work

using boost::ref;
using boost::bind;

using std::string;

static QList<QString> s_column_names_;
static QList<int> s_student_scores_;

StartTest::StartTest(app_core::AppCore* const app_ptr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    app_ptr_(app_ptr)
{
  ui->setupUi(this);

  // fill lists
  s_column_names_.append("id");
  s_column_names_.append("name");
  s_column_names_.append("priority");

  // table
  QWidget* centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  scoreTable_ = new QTableWidgetCheckEdited(this);
  scoreTable_->setColumnCount(3);
  scoreTable_->setHorizontalHeaderLabels(s_column_names_);

  // control
  QPushButton* submit = new QPushButton("Add records", this);
  connect(submit, SIGNAL(clicked(bool)), this, SLOT(slotAddRecords(bool)));

  connect(scoreTable_, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(slotRowIsChanged(QTableWidgetItem*)));

  // pack all
  QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
  mainLayout->addWidget(scoreTable_);
  mainLayout->addWidget(submit);
}

StartTest::~StartTest()
{
    delete ui;
}

void StartTest::clearList() {
  // есть и функция clear and clearContent

  int count_rows = scoreTable_->rowCount();
  for (int i = 0; i < count_rows; ++i)
    scoreTable_->removeRow(i);
}

void StartTest::slotAddRecords(bool checked) {
  domain::TasksMirror mirror(test_help_data::build_fake_model());

  // сохраняем все
  adobe::for_each(mirror, bind(&AppCore::append, ref(*app_ptr_), _1));
}

void StartTest::updateAction() {
  // FIXME: не лучший вариант все же, лучше реюзать, но как пока не ясно
  clearList();

  TasksMirror records = app_ptr_->get_current_model_data();

  {
    // fill table
    scoreTable_->setRowCount(records.size());

    // FIXME: where save id's if need it
    //scoreTable_->setVerticalHeaderLabels(s_student_names_);

    // draw one row!!
    // FIXME: лучше это сконнектить!! операция логически неделимая
    //   в принципе можно и слот на изменение сделать один.
    int pos = 0;
    for (TasksMirror::const_iterator i=records.begin(), end=records.end();
         i != end; ++i)
      {
        // id
        int id = (*i)->get_primary_key();
        QTableWidgetItem* id_item = new QTableWidgetItem(QString::number(id));
        scoreTable_->setItem(pos, 0, id_item);

        // task description
        string task_name = (*i)->get_task_name();
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromUtf8(task_name.c_str()));
        scoreTable_->setItem(pos, 1, item);

        // priority
        int priority = (*i)->get_priority();
        QTableWidgetItem* priority_item = new QTableWidgetItem(QString::number(priority));
        scoreTable_->setItem(pos, 2, priority_item);

        ++pos;
    }
  }
}

void StartTest::slotRowIsChanged(QTableWidgetItem* item)
{
  // FIXME: проблема!! изменения любые! может зациклить
  if (scoreTable_->edited()) {
    // поле было одновлено
    qDebug() << "edited " << item->row();
  }
}
