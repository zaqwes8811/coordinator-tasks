// test_emb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Python.h>
#include <iostream>
#include <windows.h>
using namespace std;


int main(int argc, char * argv[])
{
  int anyKey;
  Py_NoSiteFlag = 1;
  Py_FrozenFlag = 1;
  Py_IgnoreEnvironmentFlag = 1;
  Py_SetPythonHome("");
  Py_SetProgramName("");
    Py_Initialize();
  ::PyRun_SimpleString("import hello as h");
  ::PyRun_SimpleString("h.hello(' vv')");
  
  //
  Py_Finalize();
  Sleep(2000);
  //cin >> anyKey;
  return 0;
}

