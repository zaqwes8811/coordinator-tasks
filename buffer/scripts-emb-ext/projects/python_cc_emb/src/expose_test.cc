#include "foo.h"
#include "foo_py.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/python.hpp>
#include <gtest/gtest.h>

#include <string>
#include <iostream>

using boost::python::object;
using boost::python::handle;
using boost::python::borrowed;
using boost::python::error_already_set;

TEST(EmbPy, RunFromString) {
  Py_Initialize();
  try {
    PyRun_SimpleString(
      "a_foo = None\n"
      "\n"
      "def setup(a_foo_from_cxx):\n"
      "  print 'setup called with', a_foo_from_cxx\n"
      "  global a_foo\n"
      "  a_foo = a_foo_from_cxx\n"
      "\n"
      "def run():\n"
      "  a_foo.doSomething()\n"
      "\n"
      "print 'main module loaded'\n"
    );

    boost::shared_ptr<Foo> ptr_cc_object = boost::make_shared<Foo>("c++");


    inithello_();
    object main = object(handle<>(borrowed(PyImport_AddModule("__main__"))));

    // pass the reference to a_cxx_foo into python:
    object setup_function = main.attr("setup");
    setup_function(ptr_cc_object);

    // now run the python 'main' function
    object run_func = main.attr("run");
    run_func();
  } catch (error_already_set) {
    PyErr_Print();
  }

  Py_Finalize();
}

TEST(EmbPy, RunFromFile) {
  Py_Initialize();
  try {
    boost::shared_ptr<Foo> ptr_cc_object = boost::make_shared<Foo>("c++");

    object main = object(handle<>(borrowed(PyImport_AddModule("__main__"))));
    boost::python::object global_namespace(main.attr("__dict__"));
    boost::python::exec_file("./scripts/foo.py", global_namespace, global_namespace);

    // pass the reference to a_cxx_foo into python:
    object setup_function = main.attr("setup");
    setup_function(ptr_cc_object);

    // now run the python 'main' function
    object run_func = main.attr("run");
    run_func();
  } catch (error_already_set) {
    PyErr_Print();
  }

  Py_Finalize();
}
