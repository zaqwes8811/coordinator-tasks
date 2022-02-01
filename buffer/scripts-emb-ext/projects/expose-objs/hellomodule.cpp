#include <iostream>
#include <string>
#define BOOST_NO_EXCEPTIONS
//#include <boost/shared_ptr.hpp>
using namespace std;
//using namespace boost;
 
int say_hello(const char* name) {
    cout << "Hello " <<  name << "!\n";
	return 1;
}

class IAppLayer {
	
};

struct A_first {
    static A_first *   create () { return new A_first; }
   std::string hello  () { return "Hello, is there anybody in there?"; }
};

/*
struct A {
    static shared_ptr<A> create () { return shared_ptr<A>(new A); }
    std::string   hello  () { return "Just nod if you can hear me!"; }
};*/



 
// Экспонирование
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
using namespace boost::python;
 
BOOST_PYTHON_MODULE(hello)
{
    def("say_hello", say_hello);
}
///*
BOOST_PYTHON_MODULE(pointer)
{
	// need other boost libs!
    class_<A_first>("A_first",no_init)
        .def("create",&A_first::create, return_value_policy<manage_new_object>())
        .staticmethod("create")
        .def("hello",&A_first::hello)
    ;
}
//*/
/*
BOOST_PYTHON_MODULE(shared_ptr)
{
    class_<A, shared_ptr<A> >("A",boost::python::no_init)
        .def("create",&A::create )
        .staticmethod("create")
        .def("hello",&A::hello)
    ;
}*/