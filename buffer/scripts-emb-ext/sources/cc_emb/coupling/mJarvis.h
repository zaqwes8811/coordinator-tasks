/*
	file : #include "mJarvis.h"

	abs. : ����������� ������ ��� �������������� ������� �����
		mLuaBind and pure exposing
*/
#ifndef MJARVIS
#define MJARVIS

#ifdef LUA_EMB
	#include <mluabind/mluabind.h>
	#include <Loki/SmartPtr.h>
#endif
#include <boost/python.hpp>

// stl
#include <string>
#include <vector>
//

namespace EmbeddedInterpreters {
// https://github.com/josephturnerjr/boost-python-tutorial.git
std::string parse_python_exception();
namespace py = boost::python;
typedef std::string LocalStr;
typedef int ErrCode;

// ����������� ���
class IJarvis {
	public :
		virtual ~IJarvis(){}

		// ����������� ������
		virtual ErrCode exhibit() = 0;
		
		// Warning : ������ ����� ��������
		// ��������� � ������� ������
		virtual ErrCode runScriptInCurrentThread(LocalStr fname) = 0;

		// ��������� � ����� ������, �� ��� ������� ����. �����. ������. - ��� �����
		virtual ErrCode runScriptInNewThread(LocalStr fname) = 0;

		// ������ ��������� �������� �� �������-������������
		virtual LocalStr getStringFrom(LocalStr nameScript, LocalStr nameVar) = 0;
		virtual std::vector<LocalStr> getArrayStringFrom(LocalStr nameScript, LocalStr nameArray) = 0;

		virtual double getDoubleFrom(LocalStr nameScript, LocalStr nameVar) = 0;
		virtual std::vector<double> getArrayDoubleFrom(LocalStr nameScript, LocalStr nameArray) = 0;

		// ��������� ������
};

#ifdef LUA_EMB
class AJarvis : public IJarvis {
	public :
		// ����������� ����� �����������
		explicit AJarvis();
		virtual ~AJarvis();

		// ����������� ������
		virtual ErrCode exhibit() = 0;
		
		// Warning : ������ ����� ��������
		// ��������� � ������� ������
		virtual ErrCode runScriptInCurrentThread(LocalStr fname) = 0;

		// ��������� � ����� ������, �� ��� ������� ����. �����. ������. - ��� �����
		virtual ErrCode runScriptInNewThread(LocalStr fname) = 0;

		// ��������� � ����� ������, � ������� ����. �����. ������.
	protected:
		::lua_State* __L;
		::mluabind::CHost* __pHost;
};
#endif

#ifdef LUA_EMB
// NO work!!
class LuaEmbInterpreter : public IEmbInterpreter {
	public : 
		LuaEmbInterpreter();
		~LuaEmbInterpreter();

		LocalStr getStringFrom(LocalStr nameScript, LocalStr nameVar);
		std::vector< LocalStr > getArrayStringFrom(LocalStr nameScript, LocalStr nameArray);
		
		double getDoubleFrom(LocalStr nameScript, LocalStr nameVar);
		std::vector< double > getArrayDoubleFrom(LocalStr nameScript, LocalStr nameArray);
	private : 
		lua_State* _L;
};
#endif

};

#endif