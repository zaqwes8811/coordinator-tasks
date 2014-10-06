#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "top/config.h"  // BAD, but Qt generate some files

#include "canary/model.h"
#include "view/view.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class Engine : public QMainWindow
{
  Q_OBJECT

public:
  // ctor/dtor/assign/copy
  Engine(models::Model* const app_ptr,
       QWidget *parent = 0);
  ~Engine();

  // actions
  void redraw();

private slots:

  // filters chain:
  void filterSortByDecreasePriority(int index);

  // other actions:
  // FIXME: а есть элемент не из той таблицы?
  // FIXME: а место ли этому слоту здесь?
  // FIXME: на один сигнал можно подвесить несколько слотов
  void slotRowIsChanged(QTableWidgetItem* item);
  void slotUpdateRow();
  void slotFillFake(bool);

private:
  Ui::MainWindow *ui;
  QMyTableView* _grid_ptr;
  models::Model* const _model_ptr;
};

#endif // MAINWINDOW_H
