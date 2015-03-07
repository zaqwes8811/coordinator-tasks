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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fake_store.h"
#include "canary/entities_and_values.h"

#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QCheckBox>
#include <boost/bind.hpp>

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
    QMainWindow(parent)
{
  ui->setupUi(this);

  _model = model_ptr;
  ui = new Ui::MainWindow;

  // FIXME: сделать что-то через editor - пока тщетно. Например как сконнектиться с отнасл. таблицей?
  //   http://qt-project.org/doc/qt-4.8/designer-using-a-ui-file.html
  //
  // table


  // pack all
  QHBoxLayout* actions_layout = new QHBoxLayout;
    // boxes
    QVBoxLayout* checkbox_layout = new QVBoxLayout;
    actions_layout->addLayout(checkbox_layout);
      QCheckBox* non_done = new QCheckBox("Non done only", this);
      connect(non_done, SIGNAL(stateChanged(int)), this, SLOT(filterOnOffDone(int)));
      checkbox_layout->addWidget(non_done);

      QCheckBox* sort_dec = new QCheckBox("Sort dec priority", this);
      connect(sort_dec, SIGNAL(stateChanged(int)), this, SLOT(filterOnOffSortByDecPriority(int)));
      checkbox_layout->addWidget(sort_dec);

      QCheckBox* done = new QCheckBox("Sort by task name", this);
      connect(done, SIGNAL(stateChanged(int)), this, SLOT(filterOnOffSortByTaskName(int)));
      checkbox_layout->addWidget(done);

    // buttons
    QVBoxLayout* buttons_layout = new QVBoxLayout;
    actions_layout->addLayout(buttons_layout);
      QPushButton* mark_done = new QPushButton("Mark done", this);
      connect(mark_done, SIGNAL(clicked()), this, SLOT(slotMarkDone()));
      buttons_layout->addWidget(mark_done);

      QPushButton* fake = new QPushButton("Reopen", this);
      connect(fake, SIGNAL(clicked()), this, SLOT(slotReopen()));
      buttons_layout->addWidget(fake);

  // добавляем чекбоксы
  QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
      _table = new QMyTableView(this);
      connect(_table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(slotRowIsChanged(QTableWidgetItem*)));

      // make checkbox
      //connect(_table->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(filterOnOffSortByDecPriority(int)));

      mainLayout->addLayout(actions_layout);
      mainLayout->addWidget(_table);


  redraw();
}

Engine::~Engine() { delete ui; }

void Engine::slotReopen() {
  Row r = getSelectedRow();
  if (r.isPresent()) {
    int row = r.get();
    // Обновляем ячейку
    _table->markReopen(row);  // no throw
    values::ImmutableTask t = _table->get_elem(row);
    _model->update(t);  // FIXME: may throw
  }
}

void Engine::_processFilter(filters::FilterPtr f, int state) {
  if (Qt::Unchecked == state) {
    _filters_chain.remove(f);
  }

  if (Qt::Checked == state) {
    _filters_chain.add(f);
  }
}

void Engine::filterOnOffDone(int state) {
  filters::FilterPtr f(new filters::DoneFilter());
  _processFilter(f, state);
  redraw();
}

void Engine::filterOnOffSortByTaskName(int state) {
  filters::FilterPtr f(new filters::SortByTaskName());
  _processFilter(f, state);
  redraw();
}

void Engine::filterOnOffSortByDecPriority(int state) {
  filters::FilterPtr f(new filters::SortByPriorityFilter());
  _processFilter(f, state);
  redraw();
}

entities::Tasks Engine::get_model_data() const {
  return _filters_chain(_model->get_current_model_data());
}

#ifndef G_I_WANT_USE_IT
void Engine::slotFillFake(bool) {
  Tasks mirror(fake_store::get_all());

  // сохраняем все
  std::for_each(mirror.begin(), mirror.end()
                , bind(&Model::append, ref(*_model),
                               bind(&entities::TaskEntity::make_value, _1)));

  ::renders::render_task_store(std::cout, *_model);
}
#endif

void Engine::redraw() {
  // FIXME: не лучший вариант все же, лучше реюзать, но как пока не ясно
  // FIXME: сбивает выбранную позицию
  //
  _table->clearList();
  Tasks records = get_model_data();  // may throw
  _table->draw(records);
}

Row Engine::getSelectedRow() const {
  QModelIndexList indexList = _table->selectionModel()->selectedIndexes();

  // Должна быть выбрана одна ячейка
  if (indexList.empty() || (indexList.size() != 1))
    return Row::absent();

  const size_t row = indexList.at(values::TaskViewTableIdx::kId).row();

  if (row >= get_model_data().size())
    return Row::absent();

  return Row::of(row);
}

void Engine::slotMarkDone() {
  Row r = getSelectedRow();
  if (r.isPresent()) {
    int row = r.get();
    // Обновляем ячейку
    _table->markDone(row);  // no throw
    values::ImmutableTask t = _table->get_elem(row);
    _model->update(t);  // FIXME: may throw
  }
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

entities::Tasks::value_type Engine::get_elem_by_id(const int id) {
  Tasks r = this->get_model_data();
  Tasks::iterator it = std::find_if(r.begin(), r.end()
                                    , bind(&entities::TaskEntity::get_primary_key, _1));

  assert(it != r.end());  // должен быть
  return *it;
}
