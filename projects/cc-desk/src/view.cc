#include "heart/config.h"

#include "view.h"
#include "model_layer/entities.h"

#include <QWidget>
#include <QHeaderView>

#include <stdexcept>
#include <cassert>

using entities::TaskViewTableIdx;
using entities::Task;
using entities::TaskEntities;

const QColor QMyTableView::kDoneColor = QColor(0, 0, 255, 127);
const QColor QMyTableView::kReopenColor = QColor(0, 0, 0, 0);  // не влияет, т.к. при перерисовке
// учитывается только done column

QMyTableView::QMyTableView(QWidget *parent)
    : QTableWidget(parent) {

  // fill lists
  QList<QString> column_names;  // FIXME: move from here
  column_names.append("id");
  column_names.append("Task name");
  column_names.append("Priority");
  column_names.append("Is Done");

  setColumnCount(column_names.size());
  setHorizontalHeaderLabels(column_names);

  setColumnHidden(TaskViewTableIdx::kId, true);  // FIXME: id's пока так
  setColumnHidden(entities::TaskViewTableIdx::kDone, true);

  // Style
  QHeaderView *v = verticalHeader();
  v->setResizeMode(QHeaderView::Fixed);
  v->setDefaultSectionSize(20);

  horizontalHeader()->setResizeMode(QHeaderView
    //::Stretch);//
    ::ResizeToContents);
  //resizeRowToContents();
  //resizeColumnsToContents();
  //setWordWrap(false);
  //setTextElideMode(Qt::ElideNone);

  //setRowHeight(0, 50);

  // FIXME: растянутую ячейку отменяте смена фильтра
}

bool QMyTableView::isEdited() const {
 if (state() == QAbstractItemView::EditingState) return true;
 else return false;
}

void QMyTableView::insertBlankRows(const int end) {
  // вставляем еще несколько рядов
  for (int row = end; row < end+models::kAddedBlankLines; ++row) {
      setItem(row, entities::TaskViewTableIdx::kId, new QTableWidgetItem(QString::number(entities::EntityStates::kInactiveKey)));
      setItem(row, entities::TaskViewTableIdx::kTaskName, new QTableWidgetItem(QString()));
      setItem(row, entities::TaskViewTableIdx::kPriority,
              new QTableWidgetItem(QString::number(entities::EntityStates::kDefaultPriority)));
      setItem(row, entities::TaskViewTableIdx::kDone, new QTableWidgetItem(QString::number(entities::EntityStates::kDefaultDone)));
  }
}

void QMyTableView::clearList() {
  // есть и функция clear and clearContent
  auto size = rowCount();
  for (int i = 0; i < size; ++i)
    removeRow(i);
}

void QMyTableView::draw(entities::TaskEntities taskEntities) {
  // fill table
  setRowCount(taskEntities.size() + models::kAddedBlankLines);

  auto row = 0;
  for (auto& entity : taskEntities) {
    setItem(row, entities::TaskViewTableIdx::kId, new QTableWidgetItem(QString::number(entity->id)));
    setItem(row, entities::TaskViewTableIdx::kPriority, new QTableWidgetItem(QString::number(entity->priority)));
    setItem(row, entities::TaskViewTableIdx::kDone, new QTableWidgetItem(QString::number(entity->done)));

    auto note = QString::fromUtf8(entity->name.c_str());
    auto v = new QTableWidgetItem(note);

    if (entity->done)
      v->setTextColor(kDoneColor);


    setItem(row, entities::TaskViewTableIdx::kTaskName, v);
    ++row;
  }

  // вставляем еще несколько рядов
  insertBlankRows(row);
}

size_t QMyTableView::GetId(const int row) const {
  int val = item(row, entities::TaskViewTableIdx::kId)->text().toInt();
  return size_t(val);
}

void QMyTableView::markDone(const int row) {
  auto v = QString::number(!entities::EntityStates::kDefaultDone);
  item(row, entities::TaskViewTableIdx::kDone)->setText(v);

  // перекрашиваем
  item(row, entities::TaskViewTableIdx::kTaskName)->setTextColor(kDoneColor);
}

void QMyTableView::markReopen(const int row) {
  auto v = QString::number(entities::EntityStates::kDefaultDone);
  item(row, entities::TaskViewTableIdx::kDone)->setText(v);
  //item(row, values::TaskViewTableIdx::kTaskName)->setTextColor(kReopenColor);
}

bool QMyTableView::IsSaved(const int row) const {
  return GetId(row) != entities::EntityStates::kInactiveKey;
}

entities::Task QMyTableView::getTask(const int row) const {
  DCHECK(row < rowCount());

  auto id = GetId(row);
  std::string name(item(row, entities::TaskViewTableIdx::kTaskName)->text().toUtf8().constData());
  int p(item(row, entities::TaskViewTableIdx::kPriority)->text().toInt());
  auto isDone = item(row, entities::TaskViewTableIdx::kDone)->text().toInt();

  entities::Task t;
  t.id = id;
  t.name = name;
  t.priority = p;
  t.done = isDone;
  return t;
}
