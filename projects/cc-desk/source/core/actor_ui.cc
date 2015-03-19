#include "heart/config.h"

#include "core/actor_ui.h"
#include "common/app_types.h"
#include "view/mainwindow.h"
#include "model_layer/isolation.h"

#include <QApplication>

class ModelListenerMediator : public isolation::ModelListener_virtual {
public:
  explicit ModelListenerMediator(UiEngine* const view) : view_(view) { }

private:
  void do_update() {
    DCHECK(view_);
    view_->redraw();
  }

  UiEngine* const view_;
};


namespace actors {
// http://qt-project.org/doc/qt-4.8/qeventloop.html#processEvents
//app.exec();  // it's trouble for Actors usige
// http://stackoverflow.com/questions/16812602/qt-main-gui-and-other-thread-events-loops
// http://doc.qt.digia.com/qq/qq27-responsive-guis.html
//
// Quit
// http://stackoverflow.com/questions/8165487/how-to-do-cleaning-up-on-exit-in-qt
void UIActor::Run(gc::SharedPtr<models::Model> modelPtr) {
  int argc = 1;
  char* argv[1] = { "none" };
  QApplication appLoop(argc, argv);

  auto enginePtr = std::make_shared<UiEngine>(modelPtr.get());

  gc::SharedPtr<isolation::ModelListener_virtual> listenerPtr(new ModelListenerMediator(enginePtr.get()));
  modelPtr->setListener(listenerPtr);

  enginePtr->setUiActor(shared_from_this());
  enginePtr->show();

  while( !m_done ) {
    // ! can't sleep or wait!
    Message msg;
    if (mq.try_pop(msg))
      msg();            // execute message

    // main event loop
    appLoop.processEvents();  // hat processor!

    //if (wasTerminated) {
    //
    //}
  } // note: last message sets done to true
}
}
