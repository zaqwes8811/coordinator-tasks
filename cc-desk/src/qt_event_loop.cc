#include "config.h"

#include "app_types.h"
#include "mainwindow.h"
#include "isolation.h"
#include "qt_event_loop.h"

#include <QApplication>

namespace actors {
class ModelListenerMediator :
    public isolation::ModelListener
{
public:
  explicit ModelListenerMediator(gc::WeakPtr<UiEngine> view)
    : m_viewPtr(view) { }

  void DrawErrorMessage(const std::string& message) OVERRIDE {
    auto c = m_viewPtr.lock();
    if (c)
      c->DrawErrorMessage(message);
  }

private:
  void do_update(entities::TaskEntities e) OVERRIDE {
    auto c = m_viewPtr.lock();
    if (c)
      c->redraw(e);
  }

  gc::WeakPtr<UiEngine> m_viewPtr;
};

static int argc = 1;
static //const // not compile
char* argv[1] = { "none" };

UiObject::UiObject(concepts::db_manager_concept_t db
                   , gc::SharedPtr<std::promise<int>> pr)
  : appLoop(argc, argv), m_pr(pr) {
  // Objects
  // Must be shared. Need for actors
  model = std::make_shared<models::Model>(db);
  ui = std::make_shared<UiEngine>(model);
  uiMediator = gc::SharedPtr<isolation::ModelListener>(new ModelListenerMediator(ui));

  // Connect
  model->SetObserver(uiMediator);

  // Work if .exec()
  //QObject::connect(&appLoop, SIGNAL(aboutToQuit()), enginePtr.get(), SLOT(doWork()));
  //appLoop.exec();
  ui->show();
}

// http://qt-project.org/doc/qt-4.8/qeventloop.html#processEvents
//app.exec();  // it's trouble for Actors usige
// http://stackoverflow.com/questions/16812602/qt-main-gui-and-other-thread-events-loops
// http://doc.qt.digia.com/qq/qq27-responsive-guis.html
//
// Quit
// http://stackoverflow.com/questions/8165487/how-to-do-cleaning-up-on-exit-in-qt
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

}
