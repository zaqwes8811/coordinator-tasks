#ifndef VIEW_UI_ACTOR_H_
#define VIEW_UI_ACTOR_H_

#include "core/concepts.h"
#include "common/error_handling.h"

#include <actors_and_workers/concurent_queues.h>

#include <iostream>
#include <memory>
#include <thread>
#include <future>

namespace actors {
class UiObject;

/**
  \attention Dark place

  \attention Only in head and shared_ptr

  \fixme How check thread id in callable member?

  \bug On off application ASan SIGSEGV detect. Think trouble in off thread
  \fixme check by TSan.

  template <typename T>  // can't
  http://stackoverflow.com/questions/17853212/using-shared-from-this-in-templated-classes

  \bug TSan: Thread leak - coupled - fut+prom+connect UI
  https://groups.google.com/forum/#!topic/thread-sanitizer/EN0Per-G3Ow

  TSAN_OPTIONS=report_thread_leaks=0 - may be false positive
*/
class UIActor : public std::enable_shared_from_this<UIActor>
{
public:
  typedef std::function<void()> Message;

  UIActor();

  void Post( Message m );

  std::future<int> RunUI(concepts::db_queries_generator_concept_t db);

  void Fork();

  void Join();

private:
  UIActor( const UIActor& );           // no copying
  void operator=( const UIActor& );    // no copying

  bool m_done;                         // le flag
  fix_extern_concurent::concurent_bounded_try_queue<Message> mq;
  std::unique_ptr<std::thread> thd;          // le thread

  void Run();

//public:  // FIXME: bad!
  std::unique_ptr<UiObject> m_ui_ptr;
};
}

#endif
