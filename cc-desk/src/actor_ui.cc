#include "config.h"

#include "app_types.h"
//#include "qt_event_loop.h"

#include <actor_ui.h>

namespace actors {
void UIActor::Run() {
  while( !m_done ) {
    try {
      {
        if (ui) {
//          auto pr = ui->off();
//          if (!ui->poll()) {
//             auto p = ui.release();
//             delete p;
//          }

//          if (!ui)
//            pr->set_value(0);
        }
      }

      // ! can't sleep or wait!
      // FIXME: It's danger work without UI
      Message msg;
      if (mq.try_pop(msg)) {
        msg();            // execute message
      }
    } catch (fatal_error&) {
      throw;
    } catch (...) {
      // FIXME: don't know what to do
      throw;  // bad
    }
  } // note: last message sets done to true
}

std::future<int> UIActor::RunUI(concepts::db_manager_concept_t db)
{
  auto pr = std::make_shared<std::promise<int>>();
  auto f = pr->get_future();

  post([db, this, pr]() {
    ui = std::unique_ptr<actors::UiObject>(new actors::UiObject());//db, pr));
  });
  return f;
}

UIActor::UIActor() : m_done(false), mq(100), ui{nullptr}
{
  thd = std::unique_ptr<std::thread>(new std::thread( [=]{ this->Run(); } ) );
}

UIActor::~UIActor() {
  post( [&]{ m_done = true; } );
  thd->join();
}

}  // space

