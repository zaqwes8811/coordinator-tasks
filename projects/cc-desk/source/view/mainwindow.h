#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "heart/config.h"  // BAD, but Qt generate some files

#include "model_layer/model.h"
#include "view/view.h"
#include "model_layer/filters.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include <QTimer>

#include <stdexcept>
#include <functional>
#include <memory>

namespace Ui {
class MainWindow;
}

namespace actors {
class UIActor;
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

class UiEngine : public QMainWindow, public std::enable_shared_from_this<UiEngine>
{
  Q_OBJECT

public:
  // typedefs
  typedef std::function<void(void)> Callable;

  // ctor/dtor/assign/copy
  UiEngine(models::Model* const app_ptr,
       QWidget *parent = 0);
  ~UiEngine();

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

  /**
    \attention
  */
  void doWork() { /* ... */ }


#ifndef G_I_WANT_USE_IT
  void slotFillFake(bool);
#endif

  // FIXME: DANGER!! при реализации фильтров сломает логику!!!
  // Жесткая привязка к списку и к цепочке фильтров
  entities::TaskEntity getTaskById(const int pos);

private:
  QTimer m_timer;

  Row getSelectedRow() const;

  void processFilter(filters::FilterPtr, int state);

  entities::TaskEntities getModelData() const;

  Ui::MainWindow *m_uiRawPtr;
  QMyTableView* m_taskTablePtr;
  models::Model* m_modelPtr;

  filters::ChainFilters m_filtersChain;

  gc::SharedPtr<actors::UIActor> m_uiActorPtr;
};

#endif // MAINWINDOW_H
