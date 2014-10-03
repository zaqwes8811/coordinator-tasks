#include "top/config.h"

#include "view.h"
#include "canary/entities_and_values.h"

#include <QWidget>
#include <QHeaderView>

#include <stdexcept>

using values::TaskTableIdx;
using values::TaskValue;
using entities::Tasks;

QMyTableView::QMyTableView(QWidget *parent)
    : QTableWidget(parent) {

  // fill lists
  column_names_.append("id");
  column_names_.append("name");
  column_names_.append("priority");

  setColumnCount(column_names_.size());
  setHorizontalHeaderLabels(column_names_);
  setColumnHidden(TaskTableIdx::kId, true);  // FIXME: id's пока так

  QHeaderView *v = verticalHeader();
  v->setResizeMode(QHeaderView::Fixed);
  v->setDefaultSectionSize(20);
}

bool QMyTableView::isEdited() const {
 if (state() == QAbstractItemView::EditingState) return true;
 else return false;
}

values::TaskValue QMyTableView::create(const int row) const {
  QString d(item(row, values::TaskTableIdx::kTaskName)->text());
  QString priority(item(row, values::TaskTableIdx::kPriority)->text());

  if (d.isEmpty() && priority.isEmpty())
    throw std::logic_error("Record is empty");  // FIXME: need think about error handling system

  int p(entities::EntitiesStates::kDefaulPriority);
  if (!priority.isEmpty())
    p = priority.toInt();

  // FIXME: no injection bad!
  return TaskValue(TaskValue::create(d.toUtf8().constData(), p));
}

void QMyTableView::insertBlankRows(const int end) {
  // вставляем еще несколько рядов
  for (int row = end; row < end+app_core::kAddedBlankLines; ++row) {
      setItem(row, values::TaskTableIdx::kId, new QTableWidgetItem(QString::number(entities::EntitiesStates::kInActiveKey)));
      setItem(row, values::TaskTableIdx::kTaskName, new QTableWidgetItem);
      setItem(row, values::TaskTableIdx::kPriority, new QTableWidgetItem);
  }
}

void QMyTableView::clearList() {
  // есть и функция clear and clearContent
  int count_rows = rowCount();
  for (int i = 0; i < count_rows; ++i)
    removeRow(i);
}

void QMyTableView::update(entities::Tasks tasks) {
  {
    // fill table
    setRowCount(tasks.size() + app_core::kAddedBlankLines);

    int row = 0;
    for (Tasks::const_iterator record=tasks.begin(), end=tasks.end(); record != end; ++record) {
      setItem(row, values::TaskTableIdx::kId, new QTableWidgetItem(QString::number((*record)->get_primary_key())));
      setItem(row, values::TaskTableIdx::kTaskName, new QTableWidgetItem(QString::fromUtf8((*record)->get_task_name().c_str())));
      setItem(row, values::TaskTableIdx::kPriority, new QTableWidgetItem(QString::number((*record)->get_priority())));
      ++row;
    }

    // вставляем еще несколько рядов
    insertBlankRows(row);
  }
}


void QMyTableView::update(const int row, entities::Tasks::value_type e) {
  QString d(item(row, values::TaskTableIdx::kTaskName)->text());
  int p(item(row, values::TaskTableIdx::kPriority)->text().toInt());

  e->set_priority(p);
  e->set_task_name(d.toUtf8().constData());
}
