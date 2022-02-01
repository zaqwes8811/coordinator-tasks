g++ -Wall -c -I C:/boost_1_47_0_gcc -I D:/Python26/include bw.cpp
g++ -shared -o mfw.dll bw.o -Wl,--out-implib, -L C:/boost_1_47_0_gcc/stage/lib libboost_python-mgw45-mt-d-1_47.a

ls
rem gcc -shared -Wl,-soname, им.so -o им.so иф.cpp -I /usr/include/python2.5/ -lboost_python