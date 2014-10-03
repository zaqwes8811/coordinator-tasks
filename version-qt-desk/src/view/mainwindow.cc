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
using entities::EntitiesStates;

using boost::ref;
using boost::bind;

using std::string;
using std::vector;

View::View(app_core::Model* const model_ptr, QWidget *parent) :
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
  //connect(submit, SIGNAL(clicked(bool)), this, SLOT(slotAddRecords(bool)));

  _grid_ptr = new QMyTableView(this);
  connect(_grid_ptr, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(slotRowIsChanged(QTableWidgetItem*)));

  // pack all
  QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

  QVBoxLayout* actions_layout = new QVBoxLayout;
  actions_layout->addWidget(mark_done);
  mainLayout->addLayout(actions_layout);
  mainLayout->addWidget(_grid_ptr);

  updateAction();
}

View::~View()
{
    delete ui;
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
  _grid_ptr->clearList();
  Tasks records = _model_ptr->get_current_model_data();  // may throw
  _grid_ptr->update(records);
}

void View::slotRowIsChanged(QTableWidgetItem* elem)
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
