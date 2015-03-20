#include "heart/config.h"

#include "core/actor_ui.h"
#include "common/app_types.h"
#include "view/mainwindow.h"
#include "model_layer/isolation.h"
#include "core/scopes.h"

#include <QApplication>

class ModelListenerMediator :
    public isolation::ModelListener
{
public:
  explicit ModelListenerMediator(gc::WeakPtr<UiEngine> view)
    : m_viewPtr(view) { }

private:
  void do_update() {
    auto c = m_viewPtr.lock();
    if (c)
      c->redraw();
  }

  gc::WeakPtr<UiEngine> m_viewPtr;
};


namespace actors {
// http://qt-project.org/doc/qt-4.8/qeventloop.html#processEvents
//app.exec();  // it's trouble for Actors usige
// http://stackoverflow.com/questions/16812602/qt-main-gui-and-other-thread-events-loops
// http://doc.qt.digia.com/qq/qq27-responsive-guis.html
//
// Quit
// http://stackoverflow.com/questions/8165487/how-to-do-cleaning-up-on-exit-in-qt
void UIActor::Run(database_app::db_manager_concept_t pool) {
  {
    // work in UI thread
    int argc = 1;
    char* argv[1] = { "none" };
    QApplication appLoop(argc, argv);

    // Objects
    // Must be shared. Need for actors
    auto model = std::make_shared<models::Model>(pool);
    auto ui = std::make_shared<UiEngine>(model);
    gc::SharedPtr<isolation::ModelListener> uiMediator(new ModelListenerMediator(ui));

    // Connect
    model->setListener(uiMediator);
    model->setUiActor(shared_from_this());
    ui->setUiActor(shared_from_this());

    // Work if .exec()
    //QObject::connect(&appLoop, SIGNAL(aboutToQuit()), enginePtr.get(), SLOT(doWork()));
    //appLoop.exec();
    ui->show();
    scopes::AppScope scope;
    while( !m_done ) {
      if (ui->isReadyToDestroy()) {
        scope.setToDone();
        break;
      }

      // main event loop
      appLoop.processEvents();  // hat processor!

      // ! can't sleep or wait!
      Message msg;
      if (mq.try_pop(msg))
        msg();            // execute message
    } // note: last message sets done to true
  }

  // UI is destroyed
  {
    // Only thread loop rest
    //while( !m_done ) {
    //  Message msg;
    //  if (mq.try_pop(msg))
    //    msg();
    //}
  }
}
}
