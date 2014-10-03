#include "top/config.h"

// App
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "test_help_data.h"
#include "canary/entities_and_values.h"

//
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <boost/bind.hpp>
#include <adobe/algorithm/for_each.hpp>

#include <string>
#include <vector>

using app_core::Model;
using entities::Tasks;  // not work

using boost::ref;
using boost::bind;

using std::string;
using std::vector;

View::View(app_core::Model* const app_ptr, QWidget *parent) :
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
  scoreTable_->setColumnCount(s_column_names_.size());
  scoreTable_->setColumnHidden(0, true);  // FIXME: id's пока так
  scoreTable_->setHorizontalHeaderLabels(s_column_names_);

  // control
  QPushButton* submit = new QPushButton("Add records", this);
  connect(submit, SIGNAL(clicked(bool)), this, SLOT(slotAddRecords(bool)));

  QPushButton* mark_done = new QPushButton("Mark done", this);
  //connect(submit, SIGNAL(clicked(bool)), this, SLOT(slotAddRecords(bool)));

  connect(scoreTable_, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(slotRowIsChanged(QTableWidgetItem*)));

  // pack all
  QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
  mainLayout->addWidget(submit);
  mainLayout->addWidget(mark_done);
  mainLayout->addWidget(scoreTable_);

}

View::~View()
{
    delete ui;
}

void View::insertBlankRows(const int end) {
  // вставляем еще несколько рядов
  for (int i = end; i < end+app_core::kAddedBlankLines; ++i) {
      QTableWidgetItem* id_item =
          new QTableWidgetItem(QString::number(entities::EntitiesStates::kInActiveKey));
      scoreTable_->setItem(i, 0, id_item);
      QTableWidgetItem* item = new QTableWidgetItem;
      scoreTable_->setItem(i, 1, item);
      QTableWidgetItem* priority_item = new QTableWidgetItem();
      scoreTable_->setItem(i, 2, priority_item);
  }
}

void View::clearList() {
  // есть и функция clear and clearContent

  int count_rows = scoreTable_->rowCount();
  for (int i = 0; i < count_rows; ++i)
    scoreTable_->removeRow(i);
}

void View::slotAddRecords(bool checked) {
  Tasks mirror(test_help_data::build_fake_model());

  // сохраняем все
  adobe::for_each(mirror, bind(&Model::append, ref(*app_ptr_), _1));
}

void View::updateAction() {
  // FIXME: не лучший вариант все же, лучше реюзать, но как пока не ясно
  // FIXME: сбивает выбранную позицию
  //
  clearList();

  Tasks records = app_ptr_->get_current_model_data();

  {
    // fill table
    scoreTable_->setRowCount(records.size() + app_core::kAddedBlankLines);

    // FIXME: where save id's if need it
    //scoreTable_->setVerticalHeaderLabels(s_student_names_);

    int pos = 0;
    for (Tasks::const_iterator i=records.begin(), end=records.end(); i != end; ++i) {
      // draw one row!!
      // FIXME: лучше это сконнектить!! операция логически неделимая
      //   в принципе можно и слот на изменение сделать один.
      //
      // id
      // FIXME: так лучше не хранить, но как быть? Как надежно хранить соответствие?
      //   в объект не включить, разве что можно сделать нисходящее преобразование
      //   хотя похоже это тоже не выход. Итого. Где хранить ключ!?
      //
      //vector<QTableWidget*> tmp;
      //tmp.push_back();  // если будет исключение, то будет утечка памяти
      // мы во владение не передали
      vector<QString> tmp;  // производительность зависит от того как реализована qstring
      tmp.push_back(QString::number((*i)->get_primary_key()));
      tmp.push_back(QString::fromUtf8((*i)->get_task_name().c_str()));
      tmp.push_back(QString::number((*i)->get_priority()));

      for (int j = 0; j < tmp.size(); j++) {
          QTableWidgetItem* item = new QTableWidgetItem(tmp.at(j));
          scoreTable_->setItem(pos, j, item);
      }
      ++pos;
    }

    // вставляем еще несколько рядов
    insertBlankRows(pos);
  }
}

void View::slotRowIsChanged(QTableWidgetItem* elem)
{
  using values::TaskValue;
  using entities::EntitiesStates;

  // FIXME: проблема!! изменения любые! может зациклить
  // FIXME: а такая вот комбинация надежно то работает?
  if (scoreTable_->edited()) {
    // надежнее всего получить ID строки, индексу я не верю.
    //   может через downcasting? RTTI in Qt кажется отключено
    // http://codebetter.com/jeremymiller/2006/12/26/downcasting-is-a-code-smell/
    int id = scoreTable_->item(elem->row(), 0)->text().toInt();

    if (id == EntitiesStates::kInActiveKey) {
      // создаем новую запись
      QString d(scoreTable_->item(elem->row(), 1)->text());
      QString priority(scoreTable_->item(elem->row(), 2)->text());
      if (d.isEmpty() && priority.isEmpty())
        return;

      int p(entities::EntitiesStates::kDefaulPriority);
      if (!priority.isEmpty())
        p = priority.toInt();

      // FIXME: no injection bad!
      TaskValue v(TaskValue::create(d.toUtf8().constData(), p));
      app_ptr_->append_value(v);
    } else {
      // просто обновляем
      Tasks::value_type e(app_ptr_->get_elem_by_pos(elem->row()));

      assert(id == e->get_primary_key());

      QString d(scoreTable_->item(elem->row(), 1)->text());
      int p(scoreTable_->item(elem->row(), 2)->text().toInt());

      e->set_priority(p);
      e->set_task_name(d.toUtf8().constData());

      // обновляем
      app_ptr_->update(e);
    }
  }
}
