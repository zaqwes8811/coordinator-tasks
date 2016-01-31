#include "config.h"

#include "threading.h"

#include <boost/lexical_cast.hpp>
#include <QtWidgets/QApplication>

#include <iostream>
#include <cassert>

using namespace std;

std::string SerialExecutor::getCurrentThreadId()
{
	return boost::lexical_cast<std::string>(std::this_thread::get_id());
}

std::string SerialExecutor::GetId()
{
  auto work = getCurrentThreadId;
  std::packaged_task<std::string()> t(work);
  std::future<std::string> f = t.get_future();

  base::Closure pkg =
		  std::bind(&std::packaged_task<std::string()>::operator(), std::ref(t));
  post(pkg);

  return f.get();
}

//================================================================

SerialExecutor* Dispatcher::db_worker = NULL;
QObject* Dispatcher::ui_worker = NULL;
static std::map<int, std::string> ids_map;

void Dispatcher::PostTask( eThreadIds id, base::Closure job )
{
	if( DB == id ){
		if( db_worker )
			db_worker->post( job );
	}
	else if( UI == id ){
		if( ui_worker )
			QApplication::postEvent( ui_worker, new JobEvent( job ) );
	}
}

void Dispatcher::setQObject( QObject* obj )
{
	ui_worker = obj;
}

void Dispatcher::create()
{
	assert( !db_worker );
	db_worker = new SerialExecutor;
	ids_map[DB] = db_worker->GetId();
	ids_map[UI] = SerialExecutor::getCurrentThreadId();
}

void Dispatcher::destroy()
{
	delete db_worker;
	db_worker = NULL;
	ui_worker = NULL;
}

std::string Dispatcher::byId( enum eThreadIds id )
{
	assert( ids_map.find( id ) != ids_map.end() );
	return ids_map[ id ];
}

