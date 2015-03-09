// UI:
// http://qt-project.org/doc/qt-4.8/qtablewidget.html#details
//
// Model:
//
// Store:
#include "top/config.h"

#include "view/mainwindow.h"
#include "canary/model.h"
#include "dal/pq_queries.h"
#include "top/app_types.h"
#include "canary/isolation.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>
#include <boost/shared_ptr.hpp>
#include <loki/ScopeGuard.h>
#include <gtest/gtest.h>
#include <boost/make_shared.hpp>
#include <actors_and_workers/concurent_queues.h>

#include <memory>
#include <cassert>
#include <iostream>
#include <thread>

using Loki::ScopeGuard;
using Loki::MakeObjGuard;

using ::isolation::ModelListenerMediatorDynPolym;

class ModelListenerMediator : public ModelListenerMediatorDynPolym {
public:
  explicit ModelListenerMediator(Engine* const view) : view_(view) {
    // не должно быть нулем
  }

private:
  void update_() {
    assert(view_);
    view_->redraw();
  }

  Engine* const view_;
};


class UIActor {
public:
  typedef std::function<void()> Message;

  // FIXME: trouble is not non-arg ctor
  explicit UIActor(boost::shared_ptr<models::Model> model_ptr)
    : done(false), mq(100)
  {
    thd = std::unique_ptr<std::thread>(new std::thread( [=]{ this->Run(model_ptr); } ) );
  }

  ~UIActor() {
    Send( [&]{
      done = true;
    } ); ;
    thd->join();
  }

  void Send( Message m )
  {
    auto r = mq.try_push( m );
  }

private:

  UIActor( const UIActor& );           // no copying
  void operator=( const UIActor& );    // no copying

  bool done;                         // le flag
  //concurent::message_queue<Message> mq;        // le queue
  fix_extern_concurent::concurent_bounded_try_queue<Message> mq;
  std::unique_ptr<std::thread> thd;          // le thread

  void Run(boost::shared_ptr<models::Model> model_ptr) {
    int argc = 1;
    char* argv[1] = { "none" };
    QApplication app(argc, argv);
    auto engine_ptr = new Engine(model_ptr.get());

    boost::shared_ptr<ModelListenerMediatorDynPolym> listener(new ModelListenerMediator(engine_ptr));
    model_ptr->set_listener(listener);

    engine_ptr->show();

    // http://qt-project.org/doc/qt-4.8/qeventloop.html#processEvents
    //app.exec();  // it's trouble for Actors usige
    while( !done ) {
      // ! can't sleep or wait!
      Message msg;
      if (mq.try_pop(msg))
        msg();            // execute message

      // main event loop
      app.processEvents();  // hat processor!
    } // note: last message sets done to true
  }
};

// Actor model troubles:
//   https://www.qtdeveloperdays.com/2013/sites/default/files/presentation_pdf/Qt_Event_Loop.pdf
//   http://blog.bbv.ch/2012/10/03/multithreaded-programming-with-qt/
//
//   http://www.christeck.de/wp/2010/10/23/the-great-qthread-mess/
//
// Why not Qt?
//   http://programmers.stackexchange.com/questions/88685/why-arent-more-desktop-apps-written-with-qt
TEST(Blocked, TestApp) {
  // work in DB thread
  storages::ConnectionPoolPtr pool(
        new pq_dal::PQConnectionsPool(models::kConnection, models::kTaskTableNameRef));


  // work in UI thread
  auto model_ptr = boost::shared_ptr<models::Model>(models::Model::createForOwn(pool));
  auto _ = MakeObjGuard(*model_ptr, &models::Model::clear_store);

  // FIXME: can't post to exist actor - it block it!
  //   May be can, but UI may be will be slow.
  {
    int argc = 1;
    char* argv[1] = { "none" };
    QApplication app(argc, argv);
    auto window = new Engine(model_ptr.get());

    boost::shared_ptr<ModelListenerMediatorDynPolym> listener(new ModelListenerMediator(window));
    model_ptr->set_listener(listener);  // bad!

    window->show();

    // http://qt-project.org/doc/qt-4.8/qeventloop.html#processEvents
    //app.exec();  // it's trouble for Actors usige
    while(true) {
      // ! can't sleep or wait!
      app.processEvents();  // hat processor!
    }
  }
}

// FIXME: posting from other threads
//   http://qt-project.org/wiki/ThreadsEventsQObjects
//
// Trouble:
// http://stackoverflow.com/questions/3629557/boost-shared-from-this
TEST(Blocked, UIActorTest) {
  // work in DB thread
  storages::ConnectionPoolPtr pool(
        new pq_dal::PQConnectionsPool(models::kConnection, models::kTaskTableNameRef));


  // work in UI thread
  auto model_ptr = boost::shared_ptr<models::Model>(models::Model::createForOwn(pool));
  //auto _ = MakeObjGuard(*model_ptr, &models::Model::clear_store);

  UIActor ui(model_ptr);  // dtor will call and app out

  ui.Send([model_ptr] {
    model_ptr->getCurrentModelData();
  });

  // FIXME: troubles with out appl.
  while(true) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); }  // bad!
}



