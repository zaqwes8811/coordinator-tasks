/*
	file : #include "mJarvis.h"

	abs. : абстрактные классы дл€ экспонировани€ классов через
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

// абстрактный дл€
class IJarvis {
	public :
		virtual ~IJarvis(){}

		// Ёкспонирует классы
		virtual ErrCode exhibit() = 0;
		
		// Warning : запуск очень разнитс€
		// запустить в текущем потоку
		virtual ErrCode runScriptInCurrentThread(LocalStr fname) = 0;

		// запустить в новом потоке, но без вызовов комп. польз. интерф. - Ёто важно
		virtual ErrCode runScriptInNewThread(LocalStr fname) = 0;

		// просто получение настроек из скрипта-конфигурации
		virtual LocalStr getStringFrom(LocalStr nameScript, LocalStr nameVar) = 0;
		virtual std::vector<LocalStr> getArrayStringFrom(LocalStr nameScript, LocalStr nameArray) = 0;

		virtual double getDoubleFrom(LocalStr nameScript, LocalStr nameVar) = 0;
		virtual std::vector<double> getArrayDoubleFrom(LocalStr nameScript, LocalStr nameArray) = 0;

		// обработка ошибок
};

#ifdef LUA_EMB
class AJarvis : public IJarvis {
	public :
		// конструктор нужно наследовать
		explicit AJarvis();
		virtual ~AJarvis();

		// Ёкспонирует классы
		virtual ErrCode exhibit() = 0;
		
		// Warning : запуск очень разнитс€
		// запустить в текущем потоку
		virtual ErrCode runScriptInCurrentThread(LocalStr fname) = 0;

		// запустить в новом потоке, но без вызовов комп. польз. интерф. - Ёто важно
		virtual ErrCode runScriptInNewThread(LocalStr fname) = 0;

		// запустить в новом потоке, с вызовом комп. польз. интерф.
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