// gui-test.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"


using namespace guitest;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Открываем интерпритатор python
	Py_Initialize();
	::PyRun_SimpleString("import udp_client as uc");
	//::PyRun_SimpleString("uc.sendMsg()");

	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());

	// Зкрываем интерпритатор
	Py_Finalize();

	return 0;
}

#pragma unmanaged
void printSimple(){
 std::string msg = "'aasfsadf'";
 std::string begin = "uc.sendMsg("+msg+")";
 //std::string s = begin+"'ффф'";
 PyRun_SimpleString(begin.c_str());
}
#pragma managed
