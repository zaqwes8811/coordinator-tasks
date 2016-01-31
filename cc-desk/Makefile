
all:
	$(CC) -c -g -std=c++11 -fPIC -I/usr/include/qt5 \
			-Isrc test_glob_dispatcher.cc 
	$(CC) -c -g -std=c++11 -fPIC -I/usr/include/qt5 -Isrc src/threading.cc
#	$(CC) -c -g -std=c++11 -fPIC -I/usr/include/qt5 -Isrc src/actor_ui.cc

	$(CC) test_glob_dispatcher.o threading.o -lstdc++ -lQt5Widgets -lQt5Core -o app


folly:
	# не смог собрать
	echo "folly"

clean:
	rm *.o app