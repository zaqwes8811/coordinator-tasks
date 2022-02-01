// testLuaBind.cpp : Defines the entry point for the console application.
//
/**
frameworks inver. deped. for cpp?
��� ������ �����(������ - ���������� ������������ ���� - �����������, � ������ ����� ��������!):
	��������������� ���������� ��� ��������������
	Lua ������������ ����������� ����? �������� �������� �������� ��������� ��������!
	
	����� ��� 
		
		������� dll � ��� ��������� � ���(� ������� �����!)
		  ������ ������ �� �++ ��� �������, �� �� ����� �������(����� �� �������)
		  ? ������������� ������ � dll

    ���� ��� ���
		�� �������� ������ ������������(����� �� json, �� ������� �������� ���)
		�������� ����� ��� ���� �������

	� ����.������ ������ ����� ����� �� ��������

	��� ��. ��� ������ ����������� ������( �������� ���������� �� ������� )?

*/

#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <luabind/luabind.hpp>
#include <luabind/object.hpp>
#include <string>
#include <windows.h>
extern "C"
{
    #include "lua.h"
}
 
#ifndef _DEBUG

#else
	#pragma comment(lib, "lua5.1.lib") // ���� ������ ���
	#pragma comment(lib, "luabindd.lib") // ���� �������
#endif

 
using namespace std;
using namespace luabind;

class LState {
	lua_State *_luaState;
public :
	LState() { _luaState = lua_open(); }
	~LState() {	lua_close(_luaState); }
	void generateObjOnLuaSide();
	void pushExistObj() {
		// ����� ���� �������� ���������!
		//mObject obj;
	//obj.setStr("in C++");

	// ���������� � ��� ������ ������
	//luabind::object luaObj = luabind::object(_luaState,&obj);
	//luaObj.push(_luaState);

	// �������� ��� ������� � ���������
    //luaObj = luabind::object(luabind::from_stack(_luaState,-1));
    //mObject* test = luabind::object_cast<mObject*>(luaObj);
	//std::cout << test->getStr();
	}
};

class mObject {
public :
	string _name;
	void setStr( string name ) { _name = name; }
	string getStr() { return _name; }
};



void LState::generateObjOnLuaSide() {
	// Connect LuaBind to this lua state
	luabind::open(_luaState);
 
	// ����������� ������
	module(_luaState)
	[
		class_<mObject>("objTest")
			.def("getStr", &mObject::getStr) // �������-���� ������
	];

	// �������� ������� �� ��� �������
	string nameScript = "test.lua";
	luaL_dofile( _luaState, nameScript.c_str( ) );
	
	//cout << "Result: " << 
		//luabind::call_function<int>(_luaState, "add", 1,2);
		//luabind::call_function<mObject>(_luaState, "add", 1,2);
		//<< endl;
}
 
int main()
{
	try {
		LState ls;
		ls.generateObjOnLuaSide();
		LState ls2;
		ls2.generateObjOnLuaSide();
	}
	catch (luabind::error& e)
	{
		std::string error = lua_tostring( e.state(), -1 );
		std::cout << error << "\n";
	}

}

