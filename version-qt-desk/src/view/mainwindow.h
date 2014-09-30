#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "top/config.h"  // BAD, but Qt generate some files

#include "canary/model.h"

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class QTableWidgetCheckEdited : public QTableWidget {
  // TableWidget troubles list:
  //
  // http://qt-project.org/forums/viewthread/7769 - Move column
  //
  // selection problem:
  //  http://stackoverflow.com/questions/11816272/qtablewidget-cellactivated-signal-not-working
  //
  // How check edited state:
  //   http://www.qtcentre.org/threads/7976-How-to-know-when-a-cell-in-a-QTableWidget-has-been-edited
  //   http://qt-project.org/forums/viewthread/31372
  //
  // Internationalization:
  //  http://stackoverflow.com/questions/4214369/how-to-convert-qstring-to-stdstring
public:
  explicit QTableWidgetCheckEdited(QWidget *parent = 0)
    : QTableWidget(parent) {}

  bool edited() const {
   if (state() == QAbstractItemView::EditingState) return true;
   else return false;
  }
};

class View : public QMainWindow
{
  Q_OBJECT

public:
  View(app_core::Model* const app_ptr, QWidget *parent = 0);
  ~View();

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

  app_core::Model* const app_ptr_;

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
