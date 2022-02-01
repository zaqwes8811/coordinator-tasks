#ifndef FOO_PY_H
#define FOO_PY_H

// Third party
#include <boost/python.hpp>

// App
#include "foo.h"

int say_hello(const char* name);

BOOST_PYTHON_MODULE(hello)
{
  boost::python::class_<Foo, boost::shared_ptr<Foo> >("Foo")
    .def("doSomething", &Foo::doSomething)
  ;
  
  boost::python::def("say_hello", say_hello);
}

#endif // FOO_PY_H
