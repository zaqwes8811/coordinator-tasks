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
#include <QCheckBox>
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


  // pack all
  QVBoxLayout* actions_layout = new QVBoxLayout;
    QCheckBox* non_done = new QCheckBox("Non done only", this);
    connect(non_done, SIGNAL(stateChanged(int)), this, SLOT(filterOnOffDone(int)));
    actions_layout->addWidget(non_done);

    QCheckBox* sort_dec = new QCheckBox("Sort dec priority", this);
    connect(sort_dec, SIGNAL(stateChanged(int)), this, SLOT(filterOnOffSortByDecPriority(int)));
    actions_layout->addWidget(sort_dec);

    //QCheckBox* done = new QCheckBox("Done only", this);
    //actions_layout->addWidget(done);

    QPushButton* mark_done = new QPushButton("Mark done", this);
    connect(mark_done, SIGNAL(clicked()), this, SLOT(slotMarkDone()));
    actions_layout->addWidget(mark_done);

//#ifndef G_I_WANT_USE_IT
    QPushButton* fake = new QPushButton("Reopen", this);
    //connect(fake, SIGNAL(clicked(bool)), this, SLOT(slotFillFake(bool)));
    actions_layout->addWidget(fake);
//#endif

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

void Engine::filterOnOffDone(int state) {
  filters::FilterPtr f(new filters::DoneFilter());
  if (Qt::Unchecked == state) {
    // del filter
    _filters_chain.remove(f);
    //return;
  }

  if (Qt::Checked == state) {
    // add filter
      _filters_chain.add(f);
    //return;
  }
  redraw();
}

Engine::~Engine()
{
    delete ui;
}

void Engine::filterOnOffSortByDecPriority(int state) {
  filters::FilterPtr f(new filters::SortByPriorityFilter());
  if (Qt::Unchecked == state) {
    // del filter
    _filters_chain.remove(f);
    //return;
  }

  if (Qt::Checked == state) {
    // add filter
      _filters_chain.add(f);
    //return;
  }

  // нужно обновить вид
  redraw();
}

entities::Tasks Engine::get_model_data() const {
  return _filters_chain(_model->get_current_model_data());
}

#ifndef G_I_WANT_USE_IT
void Engine::slotFillFake(bool) {
  Tasks mirror(fake_store::get_all());

  // сохраняем все
  adobe::for_each(mirror, bind(&Model::append, ref(*_model),
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

entities::Tasks::value_type Engine::get_elem_by_id(const int id) {
  Tasks r = this->get_model_data();
  Tasks::iterator it = adobe::find_if(r, bind(&entities::TaskEntity::get_primary_key, _1));

  assert(it != r.end());  // должен быть
  return *it;
}
