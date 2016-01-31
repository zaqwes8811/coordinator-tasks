#ifndef AAW_ARCH_H_
#define AAW_ARCH_H_

#include "actors_cc11.h"

#include <QtCore/QEvent>

#include <algorithm>
#include <map>
#include <string>
#include <future>

#ifndef FROM_HERE
#define FROM_HERE ""
#endif

//============================================================

namespace base
{
// лучше не std::function
typedef std::function<void()> Closure;
typedef std::function<void(bool)> Callback;  // fixme: как тут быть? перегрузка?
}

class SerialExecutor
{
public:
  SerialExecutor() { }

  static std::string getCurrentThreadId();

  void post(base::Closure task) {
    m_pool.post(task);
  }

  std::string GetId();

private:
  cc11::Actor m_pool;
};

//====================================================

static const auto callable_type = QEvent::Type(1001);

class JobEvent : public QEvent
{
public:
	JobEvent( base::Closure job ) : QEvent( callable_type )
	{
		this->job = job;
	}

	void execute()
	{
		job();
	}
private:
	base::Closure job;
};

class Dispatcher
{
public:
	enum eThreadIds { DB, UI };

	static void PostTask(eThreadIds id, base::Closure fun);

	static void setQObject( QObject* obj );
	static void create();
	static void destroy();
  	static std::string byId( enum eThreadIds id );

private:
    Dispatcher();

	static SerialExecutor* db_worker;  // make weak access
	static QObject* ui_worker;
};

#define DCHECKTHREAD( id ) \
	assert( SerialExecutor::getCurrentThreadId() == Dispatcher::byId( id ));

#endif
