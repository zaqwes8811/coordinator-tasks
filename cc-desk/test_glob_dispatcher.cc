#include "config.h"

#include <actors_and_workers/arch.h>

#include <QtWidgets/QApplication>

#include <thread>
#include <future>

int main(int argc, char **argv)
{
	QApplication app (argc, argv);

	// fixme: make thread and put message to main loop as Callable

	return app.exec();
}
