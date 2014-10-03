#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "top/config.h"  // BAD, but Qt generate some files

#include "canary/model.h"

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class QMyTableView : public QTableWidget {
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
  explicit QMyTableView(QWidget *parent = 0);

  bool isEdited() const;
  void insertBlankRows(const int);

  // очищает список задач
  void clearList();

  void update(entities::Tasks tasks);

  values::TaskValue create(const int row) const;

private:
  // FIXME: как узнать, что ячейка именно обновлена
  //   если вошли в ячейку, значит по выходу она будет обновлена?
  //bool entered_;  //
  struct Point {
    Point() : c(0), r(0) { }
    int c;
    int r;
  };

  QList<QString> column_names_;  // FIXME: move from here
};

class View : public QMainWindow
{
  Q_OBJECT

public:
  View(app_core::Model* const app_ptr, QWidget *parent = 0);
  ~View();
  void updateAction();

private slots:
  void slotSortByDecreasePriority(bool checked);
  void slotRowIsChanged(QTableWidgetItem* item);

private:
  Ui::MainWindow *ui;
  QMyTableView* grid_ptr_;
  app_core::Model* const model_ptr_;
};

#endif // MAINWINDOW_H
