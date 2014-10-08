#include "top/config.h"

#include "view.h"
#include "canary/entities_and_values.h"

#include <QWidget>
#include <QHeaderView>

#include <stdexcept>

using values::TaskViewTableIdx;
using values::ImmutableTask;
using entities::Tasks;

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

  //setColumnHidden(TaskViewTableIdx::kId, true);  // FIXME: id's пока так
  //setColumnHidden(values::TaskViewTableIdx::kDone, true);

  // Style
  QHeaderView *v = verticalHeader();
  v->setResizeMode(QHeaderView::Fixed);
  v->setDefaultSectionSize(20);

  horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  //resizeRowToContents();
  //resizeColumnsToContents();
}

bool QMyTableView::isEdited() const {
 if (state() == QAbstractItemView::EditingState) return true;
 else return false;
}

void QMyTableView::_insertBlankRows(const int end) {
  // вставляем еще несколько рядов
  for (int row = end; row < end+models::kAddedBlankLines; ++row) {
      setItem(row, values::TaskViewTableIdx::kId, new QTableWidgetItem(QString::number(entities::EntitiesStates::kInActiveKey)));
      setItem(row, values::TaskViewTableIdx::kTaskName, new QTableWidgetItem);
      setItem(row, values::TaskViewTableIdx::kPriority, new QTableWidgetItem);
      setItem(row, values::TaskViewTableIdx::kPriority, new QTableWidgetItem(QString::number(entities::EntitiesStates::kNonDone)));
  }
}

void QMyTableView::clearList() {
  // есть и функция clear and clearContent
  int count_rows = rowCount();
  for (int i = 0; i < count_rows; ++i)
    removeRow(i);
}

void QMyTableView::draw(entities::Tasks tasks) {
  // fill table
  setRowCount(tasks.size() + models::kAddedBlankLines);

  int row = 0;
  for (Tasks::const_iterator record=tasks.begin(), end=tasks.end(); record != end; ++record) {
    setItem(row, values::TaskViewTableIdx::kId, new QTableWidgetItem(QString::number((*record)->get_primary_key())));
    setItem(row, values::TaskViewTableIdx::kTaskName, new QTableWidgetItem(QString::fromUtf8((*record)->get_task_name().c_str())));
    setItem(row, values::TaskViewTableIdx::kPriority, new QTableWidgetItem(QString::number((*record)->get_priority())));
    setItem(row, values::TaskViewTableIdx::kDone, new QTableWidgetItem(QString::number((*record)->get_is_done())));
    ++row;
  }

  // вставляем еще несколько рядов
  _insertBlankRows(row);
}

int QMyTableView::getId(const int row) const {
  return item(row, values::TaskViewTableIdx::kId)->text().toInt();
}

void QMyTableView::mark_done(const int row) {
  item(row, values::TaskViewTableIdx::kDone)->setText(QString::number(!entities::EntitiesStates::kNonDone));
}

values::ImmutableTask QMyTableView::create(const int row) const {
  QString d(item(row, values::TaskViewTableIdx::kTaskName)->text());
  QString priority(item(row, values::TaskViewTableIdx::kPriority)->text());

  if (d.isEmpty() && priority.isEmpty())
    throw std::logic_error("Record is empty");  // FIXME: need think about error handling system

  int p(entities::EntitiesStates::kDefaultPriority);
  if (!priority.isEmpty())
    p = priority.toInt();

  // FIXME: no injection bad!
  return ImmutableTask::create(d.toUtf8().constData(), p);
}

values::ImmutableTask QMyTableView::get_elem(const int row) const {
  assert(row < rowCount());

  int id = getId(row);
  QString d(item(row, values::TaskViewTableIdx::kTaskName)->text());
  int p(item(row, values::TaskViewTableIdx::kPriority)->text().toInt());
  bool done = item(row, values::TaskViewTableIdx::kPriority)->text().toInt();

  return values::ImmutableTask::create(id, d.toUtf8().constData(), p, done);
}
