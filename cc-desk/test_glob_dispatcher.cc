#include "config.h"

#include "threading.h"

#include <QtWidgets/QApplication>
#include <QtCore/QEvent>

#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <functional>

using namespace std;

// fixme: поймать сигналы
class Scope
{
public:
	Scope( QObject* obj )
	{
		Dispatcher::setQObject( obj );
		Dispatcher::create();
	}

	~Scope()
	{
		Dispatcher::destroy();  // как это согласуется с сигналами?
	}
};

class QtExecutor : public QApplication
{
public:
	QtExecutor( int argc, char **argv ) :
		QApplication( argc, argv )
	{

	}

private:
	void customEvent(QEvent* e)
	{
		if( e->type() == callable_type ){
			auto jobevent = (JobEvent*)e;
			jobevent->execute();
		}
	};
};

void job();

class Window
{
public:
	void done( int val )
	{
		DCHECKTHREAD( Dispatcher::UI );
		Dispatcher::PostTask( Dispatcher::DB, job );
	}
};

Window gw;

void job( )
{
	DCHECKTHREAD( Dispatcher::DB );

	this_thread::sleep_for( chrono::seconds( 1 ) );
	int val = 10;
	Dispatcher::PostTask( Dispatcher::UI, bind( &Window::done, gw, val ) );
}

int main(int argc, char **argv)
{
	QtExecutor app( argc, argv );
	Scope scope( &app );

	Dispatcher::PostTask( Dispatcher::DB, job );

	return app.exec();
}
