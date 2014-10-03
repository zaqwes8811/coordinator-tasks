// draw one row
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
#include <stdexcept>

using app_core::Model;
using entities::Tasks;  // not work
using values::TaskTableIdx;
using values::TaskValue;

using boost::ref;
using boost::bind;

using std::string;
using std::vector;

QMyTableView::QMyTableView(QWidget *parent)
    : QTableWidget(parent) {

  // fill lists
  column_names_.append("id");
  column_names_.append("name");
  column_names_.append("priority");

  setColumnCount(column_names_.size());
  setHorizontalHeaderLabels(column_names_);
  setColumnHidden(TaskTableIdx::kId, true);  // FIXME: id's пока так
}

bool QMyTableView::isEdited() const {
 if (state() == QAbstractItemView::EditingState) return true;
 else return false;
}

values::TaskValue QMyTableView::create(const int row) const {
  QString d(item(row, values::TaskTableIdx::kTaskName)->text());
  QString priority(item(row, values::TaskTableIdx::kPriority)->text());

  if (d.isEmpty() && priority.isEmpty())
    throw std::logic_error("Record is empty");  // FIXME: need think about error handling system

  int p(entities::EntitiesStates::kDefaulPriority);
  if (!priority.isEmpty())
    p = priority.toInt();

  // FIXME: no injection bad!
  return TaskValue(TaskValue::create(d.toUtf8().constData(), p));
}

View::View(app_core::Model* const app_ptr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model_ptr_(app_ptr)
{
  ui->setupUi(this);

  // table
  QWidget* centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  // control
  QPushButton* submit = new QPushButton("Sort by decrease priority", this);
  connect(submit, SIGNAL(clicked(bool)), this, SLOT(slotSortByDecreasePriority(bool)));

  QPushButton* mark_done = new QPushButton("Mark done", this);
  //connect(submit, SIGNAL(clicked(bool)), this, SLOT(slotAddRecords(bool)));

  grid_ptr_ = new QMyTableView(this);
  connect(grid_ptr_, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(slotRowIsChanged(QTableWidgetItem*)));

  // pack all
  QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

  QVBoxLayout* actions_layout = new QVBoxLayout;//(mainLayout);
  actions_layout->addWidget(submit);
  actions_layout->addWidget(mark_done);

  mainLayout->addLayout(actions_layout);
  mainLayout->addWidget(grid_ptr_);

  // Add empty lines - нужно загрузить старые, но как
  updateAction();
}

View::~View()
{
    delete ui;
}

void QMyTableView::insertBlankRows(const int end) {
  // вставляем еще несколько рядов
  for (int row = end; row < end+app_core::kAddedBlankLines; ++row) {
      setItem(row, values::TaskTableIdx::kId, new QTableWidgetItem(QString::number(entities::EntitiesStates::kInActiveKey)));
      setItem(row, values::TaskTableIdx::kTaskName, new QTableWidgetItem);
      setItem(row, values::TaskTableIdx::kPriority, new QTableWidgetItem);
  }
}

void QMyTableView::clearList() {
  // есть и функция clear and clearContent
  int count_rows = rowCount();
  for (int i = 0; i < count_rows; ++i)
    removeRow(i);
}

void QMyTableView::update(entities::Tasks tasks) {
  {
    // fill table
    setRowCount(tasks.size() + app_core::kAddedBlankLines);

    int row = 0;
    for (Tasks::const_iterator record=tasks.begin(), end=tasks.end(); record != end; ++record) {
      setItem(row, values::TaskTableIdx::kId, new QTableWidgetItem(QString::number((*record)->get_primary_key())));
      setItem(row, values::TaskTableIdx::kTaskName, new QTableWidgetItem(QString::fromUtf8((*record)->get_task_name().c_str())));
      setItem(row, values::TaskTableIdx::kPriority, new QTableWidgetItem(QString::number((*record)->get_priority())));
      ++row;
    }

    // вставляем еще несколько рядов
    insertBlankRows(row);
  }
}

void View::slotSortByDecreasePriority(bool checked) {
  //Tasks mirror(test_help_data::build_fake_model());

  // сохраняем все
  //adobe::for_each(mirror, bind(&Model::append, ref(*app_ptr_), _1));
}

void View::updateAction() {
  // FIXME: не лучший вариант все же, лучше реюзать, но как пока не ясно
  // FIXME: сбивает выбранную позицию
  //
  grid_ptr_->clearList();
  Tasks records = model_ptr_->get_current_model_data();  // may throw
  grid_ptr_->update(records);
}

void View::slotRowIsChanged(QTableWidgetItem* elem)
{
  using values::TaskValue;
  using entities::EntitiesStates;

  try {

  // FIXME: проблема!! изменения любые! может зациклить
  // FIXME: а такая вот комбинация надежно то работает?
  if (grid_ptr_->isEdited()) {
    int row = elem->row();

    // надежнее всего получить ID строки, индексу я не верю.
    //   может через downcasting? RTTI in Qt кажется отключено
    // http://codebetter.com/jeremymiller/2006/12/26/downcasting-is-a-code-smell/
    int id = elem->text().toInt();

    if (id == EntitiesStates::kInActiveKey) {
      // создаем новую запись
      TaskValue v = grid_ptr_->create(row);  // may throw
      model_ptr_->append_value(v);
    } else {
      // просто обновляем
      Tasks::value_type e(model_ptr_->get_elem_by_pos(row));

      assert(id == e->get_primary_key());

      QString d(grid_ptr_->item(row, values::TaskTableIdx::kTaskName)->text());
      int p(grid_ptr_->item(row, values::TaskTableIdx::kPriority)->text().toInt());

      e->set_priority(p);
      e->set_task_name(d.toUtf8().constData());

      model_ptr_->update(e);
    }
  }

  } catch (...) {
    // но как понять какое произошло
  }
}
