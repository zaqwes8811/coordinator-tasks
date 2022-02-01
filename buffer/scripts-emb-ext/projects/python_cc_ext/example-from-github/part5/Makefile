include ../Make.defaults
include ../Make.rules

OBJS = handle_error.o two_threads.o

all: parse_config

parse_config: $(OBJS) $(BOOST_PYTHON_LIB)
	$(LD) -o two_threads $^ $(LDFLAGS) -lpython2.6 

clean:
	rm -rf $(OBJS) two_threads



    
