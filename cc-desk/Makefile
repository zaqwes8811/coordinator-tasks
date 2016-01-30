
all:
	$(CC) -c -g -std=c++11 -fPIC -I/usr/include/qt5 \
			-Isrc test_glob_dispatcher.cc 
	$(CC) -c -g -std=c++11 -fPIC -Isrc src/actors_and_workers/arch.cc
	$(CC) -c -g -std=c++11 -fPIC -Isrc src/actor_ui.cc

	$(CC) test_glob_dispatcher.o arch.o actor_ui.o -lstdc++ -lQt5Widgets -lQt5Core -o app

clean:
	rm *.o app