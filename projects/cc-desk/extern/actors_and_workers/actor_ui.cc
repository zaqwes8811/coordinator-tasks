#include "heart/config.h"

#include "common/app_types.h"
#include "view/qt_event_loop.h"

#include <actors_and_workers/actor_ui.h>

namespace actors {
void UIActor::Run() {
  while( !m_done ) {
    try {
      {
        if (m_ui_ptr) {
          auto pr = m_ui_ptr->off();
          if (!m_ui_ptr->poll()) {
            //std::auto_ptr<UiObject> _(m_ui_ptr.release());
            //auto p = std::unique_ptr<UiObject>();

            auto p = m_ui_ptr.release();
            delete p;

            pr->set_value(0);
          }
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

  std::cout << "out ui thread: " << std::this_thread::get_id() << std::endl;
}

std::future<int> UIActor::RunUI(concepts::db_queries_generator_concept_t db) {
  auto pr = std::make_shared<std::promise<int>>();
  auto f = pr->get_future();
  auto self = shared_from_this();

  // FIXME: some here is bug
  post([db, self, pr]() {
    // TSan mention here
    self->m_ui_ptr = std::unique_ptr<actors::UiObject>(new actors::UiObject(db, pr));
  });
  return f;
}

UIActor::UIActor() : m_done(false), mq(100), m_ui_ptr{nullptr}
{
  // FIXME: it's bad - expose unconstuctored object
  thd = std::unique_ptr<std::thread>(new std::thread( [=]{ this->Run(); } ) );
}

UIActor::~UIActor() {
  post( [&]{
    m_done = true;
  } );
  thd->join();
}

}  // space

