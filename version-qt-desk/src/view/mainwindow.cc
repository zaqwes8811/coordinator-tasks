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
//
// надежнее всего получить ID строки, индексу я не верю.
//   может через downcasting? RTTI in Qt кажется отключено
// http://codebetter.com/jeremymiller/2006/12/26/downcasting-is-a-code-smell/

#include "top/config.h"

// App
#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "fake_store.h"
#include "canary/entities_and_values.h"

//
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <boost/bind.hpp>
#include <adobe/algorithm/for_each.hpp>
#include <adobe/algorithm/find.hpp>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

using models::Model;
using entities::Tasks;  // not work
using values::TaskViewTableIdx;
using values::ImmutableTask;
using entities::EntitiesStates;

using boost::ref;
using boost::bind;

using std::string;
using std::vector;

Engine::Engine(models::Model* const model_ptr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _model(model_ptr)
{
  ui->setupUi(this);

  // FIXME: сделать что-то через editor - пока тщетно. Например как сконнектиться с отнасл. таблицей?
  //   http://qt-project.org/doc/qt-4.8/designer-using-a-ui-file.html
  //
  // table
  QWidget* centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  // control
  QPushButton* mark_done = new QPushButton("Mark done", this);
  QPushButton* fake = new QPushButton("Fake", this);
  connect(fake, SIGNAL(clicked(bool)), this, SLOT(slotFillFake(bool)));
  connect(mark_done, SIGNAL(clicked()), this, SLOT(slotMarkDone()));

  _table = new QMyTableView(this);
  connect(_table, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(slotRowIsChanged(QTableWidgetItem*)));

  // pack all
  QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

  QVBoxLayout* actions_layout = new QVBoxLayout;
  actions_layout->addWidget(mark_done);
  actions_layout->addWidget(fake);

  // добавляем чекбоксы

  mainLayout->addWidget(_table);
  mainLayout->addLayout(actions_layout);


  connect(_table->horizontalHeader(), SIGNAL(sectionClicked(int)),
          this, SLOT(filterSortByDecPriority(int)));

  redraw();
}

Engine::~Engine()
{
    delete ui;
}

void Engine::filterSortByDecPriority(int idx) {
  if (idx == values::TaskViewTableIdx::kPriority) {
    filters::FilterPtr f(new filters::SortByPriorityFilter());
    _filters_chain.add(f);

    // нужно обновить вид
    _model->notify();
  }
}

entities::Tasks Engine::get_model_data() const {
  return _filters_chain(_model->get_current_model_data());
}

void Engine::slotFillFake(bool) {
  Tasks mirror(fake_store::get_all());

  // сохраняем все
  adobe::for_each(mirror, bind(&Model::append, ref(*_model),
                               bind(&entities::TaskEntity::make_value, _1)));

  ::renders::render_task_store(std::cout, *_model);
}

void Engine::redraw() {
  // FIXME: не лучший вариант все же, лучше реюзать, но как пока не ясно
  // FIXME: сбивает выбранную позицию
  //
  _table->clearList();
  Tasks records = get_model_data();  // may throw
  _table->draw(records);
}

void Engine::slotMarkDone() {
  QModelIndexList indexList = _table->selectionModel()->selectedIndexes();

  // Должна быть выбрана одна ячейка
  if (indexList.empty() || (indexList.size() != 1))
    return;

  const size_t row = indexList.at(values::TaskViewTableIdx::kId).row();

  if (row >= get_model_data().size())
    return;

  // Обновляем ячейку
  _table->markDone(row);  // no throw
  values::ImmutableTask t = _table->get_elem(row);
  _model->update(t);  // FIXME: may throw
}

void Engine::slotRowIsChanged(QTableWidgetItem* widget)
{
  // FIXME: проблема!! изменения любые! может зациклить
  try {
    // FIXME: а такая вот комбинация надежно то работает?
    if (_table->isEdited()) {
      const int row = widget->row();
      values::ImmutableTask v = _table->get_elem(row);
      if (_table->isSaved(row)) {
        // Cоздаем новую запись
        _model->update(v);
      } else {
        // Одна из видимых ячеек была обновлена
        _model->append(v);
      }
    }
  } catch (...) {
    // FIXME: но как понять какое произошло
  }
}

void Engine::stable_sort_decrease_priority() {
  /*adobe::stable_sort(tasks_,
      bind(std::greater<int>(),
           bind(&TaskEntity::get_priority, _1),
           bind(&TaskEntity::get_priority, _2)));

  notify();
  */
}

entities::Tasks::value_type Engine::get_elem_by_id(const int id) {
  Tasks r = this->get_model_data();
  Tasks::iterator it = adobe::find_if(r, bind(&entities::TaskEntity::get_primary_key, _1));

  assert(it != r.end());  // должен быть
  return *it;
}
