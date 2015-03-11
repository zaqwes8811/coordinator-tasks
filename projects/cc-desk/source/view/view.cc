#include "heart/config.h"

#include "view.h"
#include "things/entities.h"
#include "things/values.h"

#include <QWidget>
#include <QHeaderView>

#include <stdexcept>
#include <cassert>

using values::TaskViewTableIdx;
using values::ImmutableTask;
using entities::Tasks;

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
  setColumnHidden(values::TaskViewTableIdx::kDone, true);

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
      setItem(row, values::TaskViewTableIdx::kId, new QTableWidgetItem(QString::number(entities::EntitiesStates::kInActiveKey)));
      setItem(row, values::TaskViewTableIdx::kTaskName, new QTableWidgetItem(QString()));
      setItem(row, values::TaskViewTableIdx::kPriority,
              new QTableWidgetItem(QString::number(entities::EntitiesStates::kDefaultPriority)));
      setItem(row, values::TaskViewTableIdx::kDone, new QTableWidgetItem(QString::number(entities::EntitiesStates::kNonDone)));
  }
}

void QMyTableView::clearList() {
  // есть и функция clear and clearContent
  auto count_rows = rowCount();
  for (int i = 0; i < count_rows; ++i)
    removeRow(i);
}

void QMyTableView::draw(entities::Tasks tasks) {
  // fill table
  setRowCount(tasks.size() + models::kAddedBlankLines);

  auto row = 0;
  for (auto& record : tasks) {
    setItem(row, values::TaskViewTableIdx::kId
            , new QTableWidgetItem(QString::number(record->get_primary_key())));
    setItem(row, values::TaskViewTableIdx::kPriority
            , new QTableWidgetItem(QString::number(record->get_priority())));
    setItem(row, values::TaskViewTableIdx::kDone
            , new QTableWidgetItem(QString::number(record->getIsDone())));

    auto note = QString::fromUtf8(record->get_task_name().c_str());
    auto v = new QTableWidgetItem(note);

    if (record->getIsDone())
      v->setTextColor(kDoneColor);


    setItem(row, values::TaskViewTableIdx::kTaskName, v);
    ++row;
  }

  // вставляем еще несколько рядов
  insertBlankRows(row);
}

int QMyTableView::getId(const int row) const {
  return item(row, values::TaskViewTableIdx::kId)->text().toInt();
}

void QMyTableView::markDone(const int row) {
  auto v = QString::number(!entities::EntitiesStates::kNonDone);
  item(row, values::TaskViewTableIdx::kDone)->setText(v);

  // перекрашиваем
  item(row, values::TaskViewTableIdx::kTaskName)->setTextColor(kDoneColor);
}

void QMyTableView::markReopen(const int row) {
  auto v = QString::number(entities::EntitiesStates::kNonDone);
  item(row, values::TaskViewTableIdx::kDone)->setText(v);
  //item(row, values::TaskViewTableIdx::kTaskName)->setTextColor(kReopenColor);
}

bool QMyTableView::isSaved(const int row) const {
  return getId(row) != entities::EntitiesStates::kInActiveKey;
}

values::ImmutableTask QMyTableView::get_elem(const int row) const {
  DCHECK(row < rowCount());

  auto id = getId(row);
  std::string d(item(row, values::TaskViewTableIdx::kTaskName)->text().toUtf8().constData());
  int p(item(row, values::TaskViewTableIdx::kPriority)->text().toInt());
  auto done = item(row, values::TaskViewTableIdx::kDone)->text().toInt();

  return values::ImmutableTask::create(id, d, p, done);
}
