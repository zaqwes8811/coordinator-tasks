
all:
	g++ -std=c++11 -I/opt/lib/openvdb/include -I/opt/lib/openexr-2.2.0/include openvdb_main.cc \
	-ltbb -L/opt/lib/openexr-2.2.0/lib -lHalf -L/opt/lib/openvdb/lib -lopenvdb
	
	
launch:
	LD_LIBRARY_PATH=/opt/lib/openvdb/lib:/opt/lib/openexr-2.2.0/lib ./a.out
	
	
clean:
	rm *.out