#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "heart/config.h"  // BAD, but Qt generate some files

#include "model_layer/model.h"
#include "view/view.h"
#include "model_layer/filters.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include <actors_and_workers/actors_cc11.h>

#include <stdexcept>
#include <functional>
#include <memory>

namespace Ui { class MainWindow; }

namespace actors { class UIActor; }

class Row {
  int idx;
  Row();
  explicit Row(int _idx);

public:
  bool isPresent() const;
  int get() const;
  static Row absent();
  static Row of(int v);
};

class UiEngine : public QMainWindow, public std::enable_shared_from_this<UiEngine>
{
  Q_OBJECT

public:
  // typedefs
  typedef std::function<void(void)> Callable;

  // ctor/dtor/assign/copy
  UiEngine(gc::SharedPtr<models::Model> app_ptr, QWidget *parent = 0);
  ~UiEngine();

  // actions
  void redraw();

  void setUiActor(gc::SharedPtr<actors::UIActor> a)
  { m_uiActorPtr = a; }

  bool isReadyToDestroy() const
  { return m_fsmToDestroy; }

private slots:

  // filters chain:
  void onOnOffSortByDecPriority(int state);
  void onOnOffDone(int state);
  void onOnOffSortByTaskName(int state);

  /** other actions:
  // FIXME: а есть элемент не из той таблицы?
  // FIXME: а место ли этому слоту здесь?
  // FIXME: на один сигнал можно подвесить несколько слотов
  */
  void onRowIsChanged(QTableWidgetItem* item);
  void onMarkDone();
  void onReopen();

#ifndef G_I_WANT_USE_IT
  void onFillFake(bool);
#endif

private:
  /**
    \brief ops on start app. Need work on ready UI
  */
  void onUiLoaded();

  void showEvent( QShowEvent* event ) override
  {
      QWidget::showEvent( event );
      //your code here
      onUiLoaded();
  }

  /**
    \attention
  */
  void onClose();

  void closeEvent(QCloseEvent *event) override
  { onClose(); }

  void action();

  void doTheThing()
  { }

  Row getSelectedRow() const;

  void processFilter(filters::FilterPtr, int state);

  entities::TaskEntities getModelData() const;

  Ui::MainWindow* m_uiRawPtr;
  QMyTableView* m_taskTablePtr;
  gc::SharedPtr<models::Model> m_modelPtr;

  filters::ChainFilters m_filtersChain;

  // Coupled with actors
  gc::SharedPtr<UiEngine> share()
  { return shared_from_this(); }

  gc::WeakPtr<actors::UIActor> m_uiActorPtr;
  cc11::Actior m_dbActor;

  // FSM:
  bool m_fsmTablesIsCreated;
  bool m_fsmToDestroy{false};

  // FIXME: DANGER!! при реализации фильтров сломает логику!!!
  // Жесткая привязка к списку и к цепочке фильтров
  entities::TaskEntity getTaskById(const int pos);
};

#endif // MAINWINDOW_H
