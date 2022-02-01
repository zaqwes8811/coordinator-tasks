// di - dependency injections

// http://www.boost.org/doc/libs/1_43_0/libs/python/doc/v2/register_ptr_to_python.html

// Danger: http://stackoverflow.com/questions/20825662/boost-python-argument-types-did-not-match-c-signature

// Third party
#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/python.hpp>
#include <boost/python/register_ptr_to_python.hpp>

using boost::shared_ptr;
using boost::python::object;
using boost::python::handle;
using boost::python::borrowed;
using boost::python::error_already_set;
using boost::python::register_ptr_to_python;
using boost::python::class_;
using boost::python::call_method;
using boost::python::def;
using boost::python::no_init;
using boost::python::wrapper;
using boost::python::init;


// Абстрактный класс
class Preamplifier {
public:
	virtual ~Preamplifier () {}
	virtual int SetChannel(const int value) = 0;
};

class PreamplifierImplFake : public Preamplifier {
public:
	PreamplifierImplFake() {}
	virtual int SetChannel(const int value) {
		channel_ = value;
		return 0;
	}
private:
	int channel_;
};

class PreamplifierImplReal : public Preamplifier {
public:
	PreamplifierImplReal() {}
	virtual int SetChannel(const int value) {
		channel_ = value;
		printf("channel = %d\n", value);
		return 0;
	}
private:
	int channel_;
	int eeprom_[20];
};

// BAD, but now bee so
class PreamplifierWrapper : public Preamplifier {
 public:
   PreamplifierWrapper(shared_ptr<Preamplifier> real_ptr) : real_ptr_(real_ptr) {
  }
   virtual int SetChannel(const int value) {
     printf("channel = %d\n", value);
    return real_ptr_->SetChannel(value);
   }
private:
  shared_ptr<Preamplifier> real_ptr_;
};

// Попытки что-то обернуть
class BaseWrap : public Preamplifier, wrapper<Preamplifier> {
	int SetChannel()
	{
	if (boost::python::override SetChannel = this->get_override("f"))
		return SetChannel(); // *note*
	return Preamplifier::SetChannel(0);
	}
	int default_f() { return this->Preamplifier::SetChannel(0); }
};

BOOST_PYTHON_MODULE(preampl)
{
	/*class_<Preamplifier, shared_ptr<Preamplifier>, boost::noncopyable>("Preamplifier", no_init)
		//.def("SetChannel", &Preamplifier::SetChannel)
		;*/
  boost::python::class_<PreamplifierImplFake, 
    boost::shared_ptr<PreamplifierImplFake>>("PreamplifierImplFake")
    .def("SetChannel", &PreamplifierImplFake::SetChannel)
  ;
}

BOOST_PYTHON_MODULE(preampl_wrapper)
{
	/*class_<Preamplifier, shared_ptr<Preamplifier>, boost::noncopyable>("Preamplifier", no_init)
		//.def("SetChannel", &Preamplifier::SetChannel)
		;*/
  boost::python::class_<PreamplifierWrapper, boost::shared_ptr<PreamplifierWrapper>>("PreamplifierImplFake", init<shared_ptr<Preamplifier>>())
    .def("SetChannel", &PreamplifierWrapper::SetChannel)
  ;
}

// shared ptr - Not builded
struct A
{
    virtual int f() { return 0; }
};

shared_ptr<A> New() { return shared_ptr<A>( new A() ); }

int Ok( const shared_ptr<A>& a ) { return a->f(); }

int Fail( shared_ptr<A>& a ) { return a->f(); }

struct A_Wrapper: A
{
    A_Wrapper(PyObject* self_): self(self_) {}
    int f() { return call_method<int>(self, "f"); }    
    int default_f() { return A::f(); }    
    PyObject* self;
};

/*
BOOST_PYTHON_MODULE(register_ptr)
{
    class_<A, A_Wrapper>("A")
        .def("f", &A::f, &A_Wrapper::default_f)
    ;
    
    //def("New", &New);
    //def("Ok", &Ok);
    //def("Fail", &Fail);
    
    register_ptr_to_python< shared_ptr<A> >();
}*/

TEST(DI, Base) {
	//Preamplifier preampl;  // не компилируется
	//PreamplifierImplFake preampl;
	PreamplifierImplReal preampl;  // создание по значению!

	// Клиентский код
	// Действия над усилителем
	preampl.SetChannel(1);
}

TEST(DI, SmartPtr) {
	//Preamplifier preampl;  // не компилируется
	//PreamplifierImplFake preampl;
	shared_ptr<Preamplifier> preampl(new PreamplifierImplFake());  // создание по значению!

	// Клиентский код
	// Действия над усилителем
	preampl->SetChannel(1);
}

TEST(DI, ErrorUsige) {

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
	  "  #a_foo.SetChannel(6)\n"
	  "  pass#a_foo.SetChannel(6)\n"
      "\n"
      "print 'main module loaded'\n"
    );

    // Not work...
    boost::shared_ptr<Preamplifier> ptr_cc_object(new PreamplifierImplFake());

    initpreampl();
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

TEST(DI, BeeSo) {

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
	  "  a_foo.SetChannel(6)\n"
	  "  pass#a_foo.SetChannel(6)\n"
      "\n"
      "print 'main module loaded'\n"
    );

    // Not work...
    shared_ptr<Preamplifier> fake(new PreamplifierImplFake);
    boost::shared_ptr<PreamplifierWrapper> ptr_cc_object(
        new PreamplifierWrapper(fake));

    initpreampl_wrapper();
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

