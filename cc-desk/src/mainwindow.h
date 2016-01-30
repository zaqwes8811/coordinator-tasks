#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"  // BAD, but Qt generate some files

#include "model.h"
#include "view.h"
#include "filters.h"

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
  void redraw(entities::TaskEntities e);

  bool isReadyToDestroy() const;

  void DrawErrorMessage(const std::string& m);

private slots:

  // filters chain:
  void onOnOffSortByDecPriority(int state);
  void OnOnOffDone(int state);
  void onOnOffSortByTaskName(int state);

  /** other actions:
    \fixme а есть элемент не из той таблицы?
    \fixme а место ли этому слоту здесь?
    \fixme на один сигнал можно подвесить несколько слотов
  */
  void onRowIsChanged(QTableWidgetItem* item);
  void onMarkDone();
  void onReopen();

  void onFillFake(bool);

private:
  /**
    \brief ops on start app. Need work on ready UI
  */
  void onUiLoaded();

  /**
    \attention
  */
  void onClose();
  void doTheThing();

  gc::SharedPtr<UiEngine> share();

  Row getSelectedRow() const;

  void processFilter(filters::FilterPtr, int state);

  gc::SharedPtr<models::Model> m_modelPtr;

  /// FSM
  bool m_fsmTablesIsCreated{false};
  bool m_fsmToDestroy{false};

  /// Qt specific
  Ui::MainWindow* m_uiRawPtr;
  QMyTableView* m_taskTablePtr;

  virtual void closeEvent(QCloseEvent *event) OVERRIDE;
  virtual void showEvent( QShowEvent* event ) OVERRIDE;
};

#endif // MAINWINDOW_H
