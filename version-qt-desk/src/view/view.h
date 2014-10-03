#ifndef VIEW_H
#define VIEW_H

#include "canary/entities_and_values.h"

#include <QTableWidget>

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

  // очищает список задач
  void clearList();
  void update(entities::Tasks tasks);
  void update(const int row, entities::Tasks::value_type e);
  values::TaskValue create(const int row) const;
  int getId(const int row) const;

private:
  void insertBlankRows(const int);
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

#endif // VIEW_H
