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
#include <QMessageBox>
#include <actors_and_workers/actor_ui.h>
#include <actors_and_workers/arch.h>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <functional>

using models::Model;
using entities::TaskEntities;  // not work
using entities::TaskViewTableIdx;
using entities::Task;
using entities::EntityStates;
using std::string;
using std::vector;
using std::ref;
using std::bind;

// FIXME: try how in Guava
class Optional {
public:
  //static Optional absent() { return -1; }
};

Row::Row() : idx(-1) { }
Row::Row(int _idx) : idx(_idx) { }

bool Row::isPresent() const {
  return idx != -1 && idx >= 0;
}

int Row::get() const {
  if (!isPresent())
    throw std::runtime_error("absent");

  return idx;
}

Row Row::absent() {
  return Row();
}

Row Row::of(int v) {
  return Row(v);
}

void UiEngine::onUiLoaded()
{
  m_modelPtr->initialize([](std::string) { ; });
}

void UiEngine::onClose() {
  m_fsmToDestroy = true;
}

void UiEngine::DrawErrorMessage(const std::string& m)
{
  // http://doc.qt.io/qt-4.8/qmessagebox.html
  QMessageBox msgBox(this);
  msgBox.setText(m.c_str());
  msgBox.exec();
}

UiEngine::~UiEngine(){ delete m_uiRawPtr; }

UiEngine::UiEngine(//scopes::AppScope s,
                   gc::SharedPtr<models::Model> model_ptr,
                   QWidget *parent) :
    QMainWindow(parent)//, m_scope(s)
{
  m_uiRawPtr = //std::unique_ptr<Ui::MainWindow>(
        new Ui::MainWindow();
      //);
  m_uiRawPtr->setupUi(this);
  m_modelPtr = model_ptr;

  //connect(&m_timer, SIGNAL(timeout()), this, SLOT(doWork()));
  //m_timer.start(1000);


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
      connect(non_done, SIGNAL(stateChanged(int)), this, SLOT(OnOnOffDone(int)));
      checkbox_layout->addWidget(non_done);

      auto sort_dec = new QCheckBox("Sort dec priority", this);
      connect(sort_dec, SIGNAL(stateChanged(int)), this, SLOT(onOnOffSortByDecPriority(int)));
      checkbox_layout->addWidget(sort_dec);

      auto done = new QCheckBox("Sort by task name", this);
      connect(done, SIGNAL(stateChanged(int)), this, SLOT(onOnOffSortByTaskName(int)));
      checkbox_layout->addWidget(done);

    // buttons
    auto buttons_layout = new QVBoxLayout;
    actions_layout->addLayout(buttons_layout);
      auto mark_done = new QPushButton("Mark done", this);
      connect(mark_done, SIGNAL(clicked()), this, SLOT(onMarkDone()));
      buttons_layout->addWidget(mark_done);

      auto fake = new QPushButton("Reopen", this);
      connect(fake, SIGNAL(clicked()), this, SLOT(onReopen()));
      buttons_layout->addWidget(fake);

  // добавляем чекбоксы
  auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto mainLayout = new QVBoxLayout(centralWidget);
      m_taskTablePtr = new QMyTableView(this);
      connect(m_taskTablePtr, SIGNAL(itemChanged(QTableWidgetItem*))
              , this, SLOT(onRowIsChanged(QTableWidgetItem*)));

      // make checkbox
      //connect(_table->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(filterOnOffSortByDecPriority(int)));

      mainLayout->addLayout(actions_layout);
      mainLayout->addWidget(m_taskTablePtr);
}

void UiEngine::onReopen() {
  auto r = getSelectedRow();
  if (r.isPresent()) {
    auto row = r.get();
    // Обновляем ячейку
    m_taskTablePtr->markReopen(row);  // no throw
    entities::Task t = m_taskTablePtr->getTask(row);

    m_modelPtr->UpdateTask(t);  // FIXME: may throw
  }
}

void UiEngine::processFilter(filters::FilterPtr f, int state) {
  if (Qt::Unchecked == state) m_modelPtr->removeFilter(f);
  if (Qt::Checked == state) m_modelPtr->addFilter(f);
}

void UiEngine::OnOnOffDone(int state) {
  filters::FilterPtr f(new filters::DoneFilter());
  processFilter(f, state);
}

void UiEngine::onOnOffSortByTaskName(int state) {
  filters::FilterPtr f(new filters::SortByTaskName());
  processFilter(f, state);
}

void UiEngine::onOnOffSortByDecPriority(int state) {
  filters::FilterPtr f(new filters::SortByPriorityFilter());
  processFilter(f, state);
}

void UiEngine::onFillFake(bool) {
  using namespace std::placeholders;
  TaskEntities mirror(fake_store::get_all());

  std::for_each(mirror.begin(), mirror.end()
                , bind(&Model::AppendNewTask, ref(*m_modelPtr),
                               bind(&entities::Task::toValue, _1)));
}


void UiEngine::closeEvent(QCloseEvent *event)
{ onClose(); }

void UiEngine::doTheThing() { }

void UiEngine::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    onUiLoaded();
}

gc::SharedPtr<UiEngine> UiEngine::share()
{ return shared_from_this(); }

bool UiEngine::isReadyToDestroy() const
{ return m_fsmToDestroy; }

void UiEngine::redraw(entities::TaskEntities e) {
  // FIXME: не лучший вариант все же, лучше реюзать, но как пока не ясно
  // FIXME: сбивает выбранную позицию
  //
  m_taskTablePtr->clearList();
  auto records = e;  // may throw
  m_taskTablePtr->draw(records);
}

Row UiEngine::getSelectedRow() const {
  auto indexList = m_taskTablePtr->selectionModel()->selectedIndexes();

  // Должна быть выбрана одна ячейка
  if (indexList.empty() || (indexList.size() != 1))
    return Row::absent();

  auto const row = indexList.at(entities::TaskViewTableIdx::kId).row();

  //if (row >= getModelData().size())
  //  return Row::absent();

  return Row::of(row);
}

void UiEngine::onMarkDone() {
  auto r = getSelectedRow();
  if (r.isPresent()) {
    auto row = r.get();
    // Обновляем ячейку
    m_taskTablePtr->markDone(row);  // no throw
    auto task = m_taskTablePtr->getTask(row);
    m_modelPtr->UpdateTask(task);  // FIXME: may throw
  }
}

void UiEngine::onRowIsChanged(QTableWidgetItem* widget)
{
  // FIXME: проблема!! изменения любые! может зациклить
  try {
    // FIXME: а такая вот комбинация надежно то работает?
    if (m_taskTablePtr->isEdited()) {
      auto const row = widget->row();
      auto task = m_taskTablePtr->getTask(row);
      if (m_taskTablePtr->IsSaved(row)) {
        // Cоздаем новую запись
        m_modelPtr->UpdateTask(task);
      } else {
        // Одна из видимых ячеек была обновлена
        m_modelPtr->AppendNewTask(task);
      }
    }
  } catch (...) {
    // FIXME: но как понять какое произошло
  }
}


