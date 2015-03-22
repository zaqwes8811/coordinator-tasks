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
#include <future>

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
// http://stackoverflow.com/questions/17853212/using-shared-from-this-in-templated-classes
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

  std::future<int> connectUI(concepts::db_manager_concept_t db) {
    auto pr = std::make_shared<std::promise<int>>();
    auto f = pr->get_future();

    post([db, this, pr]() {
      uiPtr = std::unique_ptr<actors::UiObject>(new actors::UiObject(db, pr));
    });
    return f;
  }

private:
  UIActor( const UIActor& );           // no copying
  void operator=( const UIActor& );    // no copying

  bool m_done;                         // le flag
  fix_extern_concurent::concurent_bounded_try_queue<Message> mq;
  std::unique_ptr<std::thread> thd;          // le thread

  void Run();

  std::unique_ptr<UiObject> uiPtr{nullptr};
};
}

#endif
