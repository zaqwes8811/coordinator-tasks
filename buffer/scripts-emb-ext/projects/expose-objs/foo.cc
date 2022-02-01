#include <iostream>
#include <string>

#define BOOST_NO_EXCEPTIONS
// http://stackoverflow.com/questions/9272648/boost-symbol-not-found

#include "foo_py.h"
using namespace boost::python;
using namespace std;
 
int say_hello(const char* name) {
    cout << "Hello " <<  name << "!\n";
	return 1;
}


namespace boost
{

#ifdef BOOST_NO_EXCEPTIONS

void throw_exception(std::exception const & e) {
    throw e;
}; // user defined

#else

//[Not user defined --Dynguss]
template<class E> inline void throw_exception(E const & e)  
{
    throw e;
}

#endif

} // namespace boost
