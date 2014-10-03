#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "top/config.h"  // BAD, but Qt generate some files

#include "canary/model.h"
#include "view/view.h"

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}



class View : public QMainWindow
{
  Q_OBJECT

public:
  View(app_core::Model* const app_ptr, QWidget *parent = 0);
  ~View();
  void updateAction();

private slots:
  void slotSortByDecreasePriority(bool checked);

  // FIXME: а есть элемент не из той таблицы?
  void slotRowIsChanged(QTableWidgetItem* item);

private:
  Ui::MainWindow *ui;
  QMyTableView* _grid_ptr;
  app_core::Model* const _model_ptr;
};

#endif // MAINWINDOW_H
