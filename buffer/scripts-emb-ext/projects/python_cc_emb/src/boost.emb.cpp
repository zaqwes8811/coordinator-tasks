// boost.emb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*
#include "mJarvis.h"
using namespace ::EmbeddedInterpreters;


#include <iostream>
#include <memory>



class PyJarvis : public IJarvis {
public :
  explicit PyJarvis(){
    // Must be called before any boost::python functions
    Py_Initialize();
  }
  ~PyJarvis(){
    // закрыть нужно, но вроде бы не об€зательно
    Py_Finalize();
  }

  // Ёкспонирует классы
  virtual ErrCode exhibit(){}
  
  // Warning : запуск очень разнитс€
  // запустить в текущем потоку
  virtual ErrCode runScriptInCurrentThread(LocalStr fname){}

  // запустить в новом потоке, но без вызовов комп. польз. интерф. - Ёто важно
  virtual ErrCode runScriptInNewThread(LocalStr fname){}

  // просто получение настроек из скрипта-конфигурации
  virtual LocalStr getStringFrom(LocalStr nameScript, LocalStr nameVar){
    return "no implement";
  }
  virtual std::vector<LocalStr> getArrayStringFrom(LocalStr nameScript, LocalStr nameArray){
    std::vector<LocalStr> result;

    return result;
  }

  virtual double getDoubleFrom(LocalStr nameScript, LocalStr nameVar){ 
    return 0;
  }
  virtual std::vector<double> getArrayDoubleFrom(LocalStr nameScript, LocalStr nameArray){
    std::vector<double> result;

    return result;
  }
};*/

/*
int main(int, char **) {
  using namespace boost::python;
  std::auto_ptr<IJarvis> jarvis( new PyJarvis() );

  try{
    Py_Initialize();
    py::object rand_mod = py::import("fake_module");
  }catch(boost::python::error_already_set const &){
    // Parse and output the exception
    std::string perror_str = parse_python_exception();
    std::cout << "Error in Python: " << perror_str << std::endl;
  }


  / *
  Py_Initialize();
  
  try {
  PyRun_SimpleString("result = 5 ** 2");
  
  object module(handle<>(borrowed(PyImport_AddModule("__main__"))));
  object dictionary = module.attr("__dict__");
  object result = dictionary["result"];
  int result_value = extract<int>(result);
  
  std::cout << result_value << std::endl;
  
  dictionary["result"] = 20;

  PyRun_SimpleString("print result");
  } catch (error_already_set) {
  PyErr_Print();
  }
  
  Py_Finalize();* /

  return 0;
}*/
