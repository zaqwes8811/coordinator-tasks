#include "heart/config.h"

#include "core/actor_ui.h"
#include "common/app_types.h"
#include "view/mainwindow.h"
#include "model_layer/isolation.h"
#include "core/scopes.h"
#include "qt_event_loop.h"

#include <QApplication>

namespace actors {
class ModelListenerMediator :
    public isolation::ModelListener
{
public:
  explicit ModelListenerMediator(gc::WeakPtr<UiEngine> view)
    : m_viewPtr(view) { }

private:
  void do_update(entities::TaskEntities e) override {
    auto c = m_viewPtr.lock();
    if (c)
      c->redraw(e);
  }

  gc::WeakPtr<UiEngine> m_viewPtr;
};

static int argc = 1;
static char* argv[1] = { "none" };

UiObject::UiObject(concepts::db_manager_concept_t db
                   , gc::SharedPtr<std::promise<int>> pr)
  : m_pr(pr), appLoop(argc, argv) {
  // Objects
  // Must be shared. Need for actors
  model = std::make_shared<models::Model>(db);
  ui = std::make_shared<UiEngine>(model);
  uiMediator = gc::SharedPtr<isolation::ModelListener>(new ModelListenerMediator(ui));

  // Connect
  model->setListener(uiMediator);

  // Work if .exec()
  //QObject::connect(&appLoop, SIGNAL(aboutToQuit()), enginePtr.get(), SLOT(doWork()));
  //appLoop.exec();
  ui->show();
}

bool UiObject::poll() {
  if (!ui)
    return false;

  if (ui->isReadyToDestroy()) {
    return false;
  }

  // main event loop
  appLoop.processEvents();  // hat processor!
  return true;
}

// http://qt-project.org/doc/qt-4.8/qeventloop.html#processEvents
//app.exec();  // it's trouble for Actors usige
// http://stackoverflow.com/questions/16812602/qt-main-gui-and-other-thread-events-loops
// http://doc.qt.digia.com/qq/qq27-responsive-guis.html
//
// Quit
// http://stackoverflow.com/questions/8165487/how-to-do-cleaning-up-on-exit-in-qt
void UIActor::Run() {
  while( !m_done ) {
    {
      if (uiPtr) {
        auto pr = uiPtr->off();
        if (!uiPtr->poll()) {
           auto p = uiPtr.release();
           delete p;
        }

        if (!uiPtr)
          pr->set_value(0);
      }
    }

    // ! can't sleep or wait!
    // FIXME: It's danger work without UI
    Message msg;
    if (mq.try_pop(msg))
      msg();            // execute message
  } // note: last message sets done to true
}

std::future<int> UIActor::RunUI(concepts::db_manager_concept_t db) {
  auto pr = std::make_shared<std::promise<int>>();
  auto f = pr->get_future();

  post([db, this, pr]() {
    uiPtr = std::unique_ptr<actors::UiObject>(new actors::UiObject(db, pr));
  });
  return f;
}

UIActor::UIActor() : m_done(false), mq(100), uiPtr{nullptr}
{ thd = std::unique_ptr<std::thread>(new std::thread( [=]{ this->Run(); } ) ); }

UIActor::~UIActor() {
  post( [&]{ m_done = true; } );
  thd->join();
}

}  // space

