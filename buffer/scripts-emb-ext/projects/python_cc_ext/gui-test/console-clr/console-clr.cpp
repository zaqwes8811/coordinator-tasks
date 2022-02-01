// console-clr.cpp : main project file.

#include "stdafx.h"
#pragma unmanaged
#include <Python.h>
#pragma managed

using namespace System;

int main(array<System::String ^> ^args)
{
  Py_Initialize();
  //PyRun_SimpleString("from time import time,ctime\n"
                  //   "print 'Today is',ctime(time())\n");
  PyRun_SimpleString("import udp_client");
  Py_Finalize();
  return 0;
}
