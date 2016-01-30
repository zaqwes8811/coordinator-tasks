#include "config.h"

#include <actors_and_workers/arch.h>

#include <QtWidgets/QApplication>
#include <QtCore/QEvent>

#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <functional>

using namespace std;

typedef function<void()> Callable;
static const auto callable_type = QEvent::Type(1001);

class JobEvent : public QEvent
{
public:
	JobEvent( Callable job ) : QEvent( callable_type )
	{
		this->job = job;
	}

	void execute()
	{
		job();
	}
private:
	Callable job;
};

class ExecutorQApplication : public QApplication
{
public:
	ExecutorQApplication( int argc, char **argv ) :
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

void thread_fn( ExecutorQApplication* app )
{
	this_thread::sleep_for( chrono::seconds( 2 ) );
	JobEvent *e = new JobEvent([] {
		cout << "hello" << endl;
	});

	QApplication::postEvent( app, e );
}

int main(int argc, char **argv)
{
	ExecutorQApplication app (argc, argv);

	thread t( [&app] { thread_fn( &app ); } );

	return app.exec();
}
