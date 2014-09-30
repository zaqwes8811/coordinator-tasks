#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "top/config.h"  // BAD, but Qt generate some files

#include "canary/app_core.h"

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class QTableWidgetCheckEdited : public QTableWidget {
public:
  explicit QTableWidgetCheckEdited(QWidget *parent = 0)
    : QTableWidget(parent) {}

  bool edited() const {
   if (state() == QAbstractItemView::EditingState) return true;
   else return false;
  }
};

class StartTest : public QMainWindow
{
  Q_OBJECT

public:
  StartTest(app_core::AppCore* const app_ptr, QWidget *parent = 0);
  ~StartTest();

  void updateAction();
  void insertBlankRows(const int);

private slots:
  void slotAddRecords(bool checked);
  void slotRowIsChanged(QTableWidgetItem* item);

private:
  // очищает список задач
  void clearList();

  Ui::MainWindow *ui;
  QTableWidgetCheckEdited* scoreTable_;

  app_core::AppCore* const app_ptr_;

  // FIXME: как узнать, что ячейка именно обновлена
  //   если вошли в ячейку, значит по выходу она будет обновлена?
  //bool entered_;  //
  struct Point {
    Point() : c(0), r(0) { }
    int c;
    int r;
  };

  QList<QString> s_column_names_;
};

#endif // MAINWINDOW_H
