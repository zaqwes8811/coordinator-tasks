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
#include <iostream>

using models::Model;
using entities::Tasks;  // not work
using values::TaskViewTableIdx;
using values::TaskValue;
using entities::EntitiesStates;

using boost::ref;
using boost::bind;

using std::string;
using std::vector;

Engine::Engine(models::Model* const model_ptr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _model_ptr(model_ptr)
{
  ui->setupUi(this);

  // table
  QWidget* centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  // control
  QPushButton* mark_done = new QPushButton("Mark done", this);
  QPushButton* fake = new QPushButton("Fake", this);
  connect(fake, SIGNAL(clicked(bool)), this, SLOT(slotFillFake(bool)));
  connect(mark_done, SIGNAL(clicked()), this, SLOT(slotUpdateRow()));

  _grid_ptr = new QMyTableView(this);
  connect(_grid_ptr, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(slotRowIsChanged(QTableWidgetItem*)));

  // pack all
  QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

  QVBoxLayout* actions_layout = new QVBoxLayout;
  actions_layout->addWidget(mark_done);
  actions_layout->addWidget(fake);

  mainLayout->addWidget(_grid_ptr);
  mainLayout->addLayout(actions_layout);


  connect(_grid_ptr->horizontalHeader(), SIGNAL(sectionClicked(int)),
          this, SLOT(filterSortByDecreasePriority(int)));

  redraw();
}

Engine::~Engine()
{
    delete ui;
}

void Engine::filterSortByDecreasePriority(int idx) {
  if (idx == values::TaskViewTableIdx::kPriority) {
    // сортируем
    _model_ptr->stable_sort_decrease_priority();
  }
}

void Engine::slotFillFake(bool) {
  Tasks mirror(test_help_data::build_fake_model());

  // сохраняем все
  adobe::for_each(mirror, bind(&Model::append, ref(*_model_ptr), _1));

  ::renders::render_task_store(std::cout, *_model_ptr);
}

void Engine::redraw() {
  // FIXME: не лучший вариант все же, лучше реюзать, но как пока не ясно
  // FIXME: сбивает выбранную позицию
  //
  _grid_ptr->clearList();
  Tasks records = _model_ptr->get_current_model_data();  // may throw
  _grid_ptr->update(records);
}

void Engine::slotUpdateRow() {
  //if (!_grid_ptr->hasSelection())
  //  return;

  QModelIndexList indexList = _grid_ptr->selectionModel()->selectedIndexes();

  // Должна быть выбрана одна ячейка
  if (indexList.empty() || (indexList.size() != 1))
    return;

  QModelIndex idx = indexList.at(0);

  const int kRow = idx.row();

  if (kRow >= _model_ptr->get_current_model_data().size())  // FIXME: BAD!!! slow
    return;

  // Обновляем ячейку
  Tasks::value_type e(_model_ptr->get_elem_by_pos(kRow));
  e->set_is_done(true);

  _model_ptr->update(e);

  ::renders::render_task_store(std::cout, *_model_ptr);
}

void Engine::slotRowIsChanged(QTableWidgetItem* elem)
{
  try {
    // FIXME: проблема!! изменения любые! может зациклить
    // FIXME: а такая вот комбинация надежно то работает?
    if (_grid_ptr->isEdited()) {
      const int kRow = elem->row();

      // надежнее всего получить ID строки, индексу я не верю.
      //   может через downcasting? RTTI in Qt кажется отключено
      // http://codebetter.com/jeremymiller/2006/12/26/downcasting-is-a-code-smell/
      const int kId = _grid_ptr->getId(kRow);

      if (kId == EntitiesStates::kInActiveKey) {
        // создаем новую запись
        TaskValue v = _grid_ptr->create(kRow);  // may throw
        _model_ptr->append_value(v);
      } else {
        // просто обновляем
        Tasks::value_type e(_model_ptr->get_elem_by_pos(kRow));

        assert(kId == e->get_primary_key());

        _grid_ptr->update(kRow, e);
        _model_ptr->update(e);
      }
    }
  } catch (...) {
    // FIXME: но как понять какое произошло
  }
}
