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
}

void UIActor::Post( Message m )
{
  try {
    auto r = mq.try_push( m );
    if (!r)
      throw infrastructure_error(FROM_HERE);
  } catch (...) {
    throw infrastructure_error(FROM_HERE);
  }
}

void UIActor::Fork() {
  auto self = shared_from_this();
  thd = std::unique_ptr<std::thread>(new std::thread( [self]{ self->Run(); } ) );
}

void UIActor::Join() {
  if (thd) {
    Post( [&]{
      m_done = true;
    } );
    thd->join();
    //thd = nullptr;
    DCHECK(!thd->joinable());
  }
}

std::future<int> UIActor::RunUI(concepts::db_queries_generator_concept_t db) {
  auto pr = std::make_shared<std::promise<int>>();
  auto f = pr->get_future();

  // FIXME: some here is bug
  Post([db, this, pr]() {
    // TSan mention here
    m_ui_ptr = std::unique_ptr<actors::UiObject>(new actors::UiObject(db, pr));
  });
  return f;
}

UIActor::UIActor() : m_done(false), mq(100), m_ui_ptr{nullptr}
{
  // FIXME: it's bad - expose unconstuctored object
  //
}
}  // space

