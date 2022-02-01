#ifndef FOO_PY_H
#define FOO_PY_H

// Third party
#include <boost/python.hpp>

// App
#include "foo.h"

BOOST_PYTHON_MODULE(hello_)
{
  boost::python::class_<Foo, boost::shared_ptr<Foo> >("Foo")
    .def("doSomething", &Foo::doSomething)
  ;
}

#endif // FOO_PY_H
