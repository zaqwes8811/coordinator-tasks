#ifndef VIEW_H
#define VIEW_H

#include "things/entities_and_values.h"

#include <QTableWidget>

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
class QMyTableView : public QTableWidget {

public:
  explicit QMyTableView(QWidget *parent = 0);
  bool isEdited() const;

  // очищает список задач
  void clearList();
  void draw(entities::Tasks tasks);

  values::ImmutableTask get_elem(const int row) const;
  int getId(const int row) const;
  bool isSaved(const int row) const;
  void markDone(const int row);
  void markReopen(const int row);

private:
  void _insertBlankRows(const int);

  static const QColor kDoneColor;
  static const QColor kReopenColor;
};

#endif // VIEW_H
