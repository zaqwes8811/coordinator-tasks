#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "top/config.h"  // BAD, but Qt generate some files

#include "canary/model.h"
#include "view/view.h"
#include "canary/filters.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include <QTimer>

#include <stdexcept>
#include <functional>

namespace Ui {
class MainWindow;
}

// FIXME: try how in Guava
class Optional {
public:
  //static Optional absent() { return -1; }
};

class Row {
  int idx;
  Row() : idx(-1) { }
  explicit Row(int _idx) : idx(_idx) { }

public:
  bool isPresent() const {
    return idx != -1 && idx >= 0;
  }

  int get() const {
    if (!isPresent())
      throw std::runtime_error("absent");

    return idx;
  }

  static Row absent() {
    return Row();
  }

  static Row of(int v) {
    return Row(v);
  }
};

class Engine : public QMainWindow
{
  Q_OBJECT

public:
  // typedefs
  typedef std::function<void(void)> Callable;

  // ctor/dtor/assign/copy
  Engine(models::Model* const app_ptr,
       QWidget *parent = 0);
  ~Engine();

  // actions
  void redraw();

private slots:

  // filters chain:
  void filterOnOffSortByDecPriority(int state);
  void filterOnOffDone(int state);
  void filterOnOffSortByTaskName(int state);

  // other actions:
  // FIXME: а есть элемент не из той таблицы?
  // FIXME: а место ли этому слоту здесь?
  // FIXME: на один сигнал можно подвесить несколько слотов
  void slotRowIsChanged(QTableWidgetItem* item);
  void slotMarkDone();
  void slotReopen();

  void doWork() {
     /* ... */
  }

#ifndef G_I_WANT_USE_IT
  void slotFillFake(bool);
#endif

  // FIXME: DANGER!! при реализации фильтров сломает логику!!!
  // Жесткая привязка к списку и к цепочке фильтров
  entities::Tasks::value_type get_elem_by_id(const int pos);

private:
  Row getSelectedRow() const;

  void _processFilter(filters::FilterPtr, int state);

  entities::Tasks get_model_data() const;

  Ui::MainWindow *ui;
  QMyTableView* _table;
  models::Model* _model;

  filters::ChainFilters _filters_chain;

  QTimer m_timer;
};

#endif // MAINWINDOW_H
