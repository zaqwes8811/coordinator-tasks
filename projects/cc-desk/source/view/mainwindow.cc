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

#include "heart/config.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "data_access_layer/fake_store.h"
#include "model_layer/entities.h"

#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QCheckBox>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <functional>

using models::Model;
using entities::TaskEntities;  // not work
using entities::TaskViewTableIdx;
using entities::TaskValue;
using entities::EntityStates;
using std::string;
using std::vector;
using std::ref;
using std::bind;

Engine::Engine(models::Model* const model_ptr, QWidget *parent) :
    QMainWindow(parent)
{
  m_uiRawPtr = new Ui::MainWindow;
  m_uiRawPtr->setupUi(this);
  m_modelPtr = model_ptr;

  connect(&m_timer, SIGNAL(timeout()), this, SLOT(doWork()));
  m_timer.start(1000);


  // FIXME: сделать что-то через editor - пока тщетно. Например как сконнектиться с отнасл. таблицей?
  //   http://qt-project.org/doc/qt-4.8/designer-using-a-ui-file.html
  //
  // table


  // pack all
  auto actions_layout = new QHBoxLayout;
    // boxes
    auto checkbox_layout = new QVBoxLayout;
    actions_layout->addLayout(checkbox_layout);
      auto non_done = new QCheckBox("Non done only", this);
      connect(non_done, SIGNAL(stateChanged(int)), this, SLOT(filterOnOffDone(int)));
      checkbox_layout->addWidget(non_done);

      auto sort_dec = new QCheckBox("Sort dec priority", this);
      connect(sort_dec, SIGNAL(stateChanged(int)), this, SLOT(filterOnOffSortByDecPriority(int)));
      checkbox_layout->addWidget(sort_dec);

      auto done = new QCheckBox("Sort by task name", this);
      connect(done, SIGNAL(stateChanged(int)), this, SLOT(filterOnOffSortByTaskName(int)));
      checkbox_layout->addWidget(done);

    // buttons
    auto buttons_layout = new QVBoxLayout;
    actions_layout->addLayout(buttons_layout);
      auto mark_done = new QPushButton("Mark done", this);
      connect(mark_done, SIGNAL(clicked()), this, SLOT(slotMarkDone()));
      buttons_layout->addWidget(mark_done);

      auto fake = new QPushButton("Reopen", this);
      connect(fake, SIGNAL(clicked()), this, SLOT(slotReopen()));
      buttons_layout->addWidget(fake);

  // добавляем чекбоксы
  auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto mainLayout = new QVBoxLayout(centralWidget);
      m_taskTablePtr = new QMyTableView(this);
      connect(m_taskTablePtr, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(slotRowIsChanged(QTableWidgetItem*)));

      // make checkbox
      //connect(_table->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(filterOnOffSortByDecPriority(int)));

      mainLayout->addLayout(actions_layout);
      mainLayout->addWidget(m_taskTablePtr);


  redraw();
}

Engine::~Engine() { delete m_uiRawPtr; }

void Engine::slotReopen() {
  auto r = getSelectedRow();
  if (r.isPresent()) {
    auto row = r.get();
    // Обновляем ячейку
    m_taskTablePtr->markReopen(row);  // no throw
    entities::TaskValue t = m_taskTablePtr->getTask(row);
    m_modelPtr->update(t);  // FIXME: may throw
  }
}

void Engine::processFilter(filters::FilterPtr f, int state) {
  if (Qt::Unchecked == state) {
    m_filtersChain.remove(f);
  }

  if (Qt::Checked == state) {
    m_filtersChain.add(f);
  }
}

void Engine::filterOnOffDone(int state) {
  filters::FilterPtr f(new filters::DoneFilter());
  processFilter(f, state);
  redraw();
}

void Engine::filterOnOffSortByTaskName(int state) {
  filters::FilterPtr f(new filters::SortByTaskName());
  processFilter(f, state);
  redraw();
}

void Engine::filterOnOffSortByDecPriority(int state) {
  filters::FilterPtr f(new filters::SortByPriorityFilter());
  processFilter(f, state);
  redraw();
}

entities::TaskEntities Engine::getModelData() const {
  return m_filtersChain(m_modelPtr->getCurrentModelData());
}

#ifndef G_I_WANT_USE_IT
void Engine::slotFillFake(bool) {
  using namespace std::placeholders;
  TaskEntities mirror(fake_store::get_all());

  // сохраняем все
  std::for_each(mirror.begin(), mirror.end()
                , bind(&Model::appendNewTask, ref(*m_modelPtr),
                               bind(&entities::TaskEntity::toValue, _1)));

  ::renders::render_task_store(std::cout, *m_modelPtr);
}
#endif

void Engine::redraw() {
  // FIXME: не лучший вариант все же, лучше реюзать, но как пока не ясно
  // FIXME: сбивает выбранную позицию
  //
  m_taskTablePtr->clearList();
  auto records = getModelData();  // may throw
  m_taskTablePtr->draw(records);
}

Row Engine::getSelectedRow() const {
  auto indexList = m_taskTablePtr->selectionModel()->selectedIndexes();

  // Должна быть выбрана одна ячейка
  if (indexList.empty() || (indexList.size() != 1))
    return Row::absent();

  auto const row = indexList.at(entities::TaskViewTableIdx::kId).row();

  if (row >= getModelData().size())
    return Row::absent();

  return Row::of(row);
}

void Engine::slotMarkDone() {
  auto r = getSelectedRow();
  if (r.isPresent()) {
    auto row = r.get();
    // Обновляем ячейку
    m_taskTablePtr->markDone(row);  // no throw
    auto task = m_taskTablePtr->getTask(row);
    m_modelPtr->update(task);  // FIXME: may throw
  }
}

void Engine::slotRowIsChanged(QTableWidgetItem* widget)
{
  // FIXME: проблема!! изменения любые! может зациклить
  try {
    // FIXME: а такая вот комбинация надежно то работает?
    if (m_taskTablePtr->isEdited()) {
      auto const row = widget->row();
      auto task = m_taskTablePtr->getTask(row);
      if (m_taskTablePtr->isSaved(row)) {
        // Cоздаем новую запись
        m_modelPtr->update(task);
      } else {
        // Одна из видимых ячеек была обновлена
        m_modelPtr->appendNewTask(task);
      }
    }
  } catch (...) {
    // FIXME: но как понять какое произошло
  }
}

entities::TaskEntities::value_type Engine::getTaskById(const int id) {
  using namespace std::placeholders;
  auto r = getModelData();
  auto it = std::find_if(r.begin(), r.end(), bind(&entities::TaskEntity::id, _1));

  DCHECK(it != r.end());  // должен быть
  return *it;
}
