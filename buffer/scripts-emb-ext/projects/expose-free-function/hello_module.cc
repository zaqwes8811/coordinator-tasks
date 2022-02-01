// C
#include <stdio.h>

#define DO_PY_WRAP
//#ifdef DO_PY_WRAP
// C++
//#include <stlport/vector>  // fail compile
#include <vector>

// Other
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
//#endif  // DO_PY_WRAP

void say_hello(const char* name) {
   printf("%s \n", name);
}

//#ifdef DO_PY_WRAP
using namespace boost::python;
// Expose
BOOST_PYTHON_MODULE(hello) {
    def("say_hello", say_hello);
}
//#endif  // DO_PY_WRAP