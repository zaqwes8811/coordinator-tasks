#ifndef VIEW_UI_ACTOR_H_
#define VIEW_UI_ACTOR_H_

#include "common/app_types.h"
#include "core/concepts.h"
#include "model_layer/model.h"
#include "core/scopes.h"
#include "view/mainwindow.h"

#include <actors_and_workers/concurent_queues.h>
#include <QApplication>

#include <iostream>
#include <memory>
#include <thread>

namespace actors {
class UiObject : public std::enable_shared_from_this<UiObject> {
public:
  UiObject(concepts::db_manager_concept_t db);
  void poll();

private:
  scopes::AppScope scope;

  gc::SharedPtr<models::Model> model;
  gc::SharedPtr<isolation::ModelListener> uiMediator;
  gc::SharedPtr<UiEngine> ui;

  QApplication appLoop;
};

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

/**
  \attention Dark place

  \attention Only in head and shared_ptr

  \fixme How check thread id in callable member?

  \bug On off application ASan SIGSEGV detect. Think trouble in off thread
  \fixme check by TSan.
*/
//template <typename T>  // can't
class UIActor : public std::enable_shared_from_this<UIActor>
{
public:
  typedef std::function<void()> Message;

  // FIXME: trouble is not non-arg ctor
  explicit UIActor() : m_done(false), mq(100)
  { thd = std::unique_ptr<std::thread>(new std::thread( [=]{ this->Run(); } ) ); }

  ~UIActor() {
    post( [&]{ m_done = true; } );
    thd->join();
  }

  void post( Message m )
  { auto r = mq.try_push( m ); }

  void connectUI(gc::SharedPtr<UiObject> ui) {

  }

  void disconnectUI() {

  }

private:
  UIActor( const UIActor& );           // no copying
  void operator=( const UIActor& );    // no copying

  bool m_done;                         // le flag
  fix_extern_concurent::concurent_bounded_try_queue<Message> mq;
  std::unique_ptr<std::thread> thd;          // le thread

  void Run();

  gc::SharedPtr<UiObject> uiPtr{nullptr};
};
}

#endif
