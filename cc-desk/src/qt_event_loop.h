#ifndef CORE_QT_EVENT_LOOP_H_
#define CORE_QT_EVENT_LOOP_H_

#include "concepts.h"
#include "app_types.h"
#include "app_types.h"
#include "concepts.h"
#include "model.h"
#include "scopes.h"
#include "mainwindow.h"

#include <QApplication>

#include <future>

// Actor model troubles:
//   https://www.qtdeveloperdays.com/2013/sites/default/files/presentation_pdf/Qt_Event_Loop.pdf
//   http://blog.bbv.ch/2012/10/03/multithreaded-programming-with-qt/
//
//   http://www.christeck.de/wp/2010/10/23/the-great-qthread-mess/
//
// Why not Qt?
//   http://programmers.stackexchange.com/questions/88685/why-arent-more-desktop-apps-written-with-qt
//
// http://elfery.net/blog/signals.html ! what wrong with Sig/Slo
//
// FIXME: posting from other threads
//   http://qt-project.org/wiki/ThreadsEventsQObjects
//
// Trouble:
// http://stackoverflow.com/questions/3629557/boost-shared-from-this
namespace actors {
class UiObject {
public:
  UiObject(concepts::db_manager_concept_t db, gc::SharedPtr<std::promise<int>> pr);
  bool poll();

  gc::SharedPtr<std::promise<int>> off() { return m_pr; }

private:
  // it first
  QApplication appLoop;

  // other after
  gc::SharedPtr<models::Model> model;
  gc::SharedPtr<isolation::ModelListener> uiMediator;
  gc::SharedPtr<UiEngine> ui;

  gc::SharedPtr<std::promise<int>> m_pr;
};
}

#endif
