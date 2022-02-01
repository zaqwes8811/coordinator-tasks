#include "mJarvis.h"
using namespace ::EmbeddedInterpreters;
using namespace std;


// Parses the value of the active python exception
// NOTE SHOULD NOT BE CALLED IF NO EXCEPTION
std::string EmbeddedInterpreters::parse_python_exception(){
    PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
    // Fetch the exception info from the Python C API
    PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);

    // Fallback error
    std::string ret("Unfetchable Python error");
    // If the fetch got a type pointer, parse the type into the exception string
    if(type_ptr != NULL){
        py::handle<> h_type(type_ptr);
        py::str type_pstr(h_type);
        // Extract the string from the boost::python object
        py::extract<std::string> e_type_pstr(type_pstr);
        // If a valid string extraction is available, use it 
        //  otherwise use fallback
        if(e_type_pstr.check())
            ret = e_type_pstr();
        else
            ret = "Unknown exception type";
    }
    // Do the same for the exception value (the stringification of the exception)
    if(value_ptr != NULL){
        py::handle<> h_val(value_ptr);
        py::str a(h_val);
        py::extract<std::string> returned(a);
        if(returned.check())
            ret +=  ": " + returned();
        else
            ret += std::string(": Unparseable Python error: ");
    }
    // Parse lines from the traceback using the Python traceback module
    if(traceback_ptr != NULL){
        py::handle<> h_tb(traceback_ptr);
        // Load the traceback module and the format_tb function
        py::object tb(py::import("traceback"));
        py::object fmt_tb(tb.attr("format_tb"));
        // Call format_tb to get a list of traceback strings
        py::object tb_list(fmt_tb(h_tb));
        // Join the traceback strings into a single string
        py::object tb_str(py::str("\n").join(tb_list));
        // Extract the string, check the extraction, and fallback in necessary
        py::extract<std::string> returned(tb_str);
        if(returned.check())
            ret += ": " + returned();
        else
            ret += std::string(": Unparseable Python traceback");
    }
    return ret;
}


#ifdef LUA_EMB
	using namespace mluabind;
#endif


#ifdef LUA_EMB
AJarvis::AJarvis() {
	__L = lua_open( );
	luaL_openlibs( __L );
	
	__pHost = new CHost( __L );
	BindStdLibrary( __pHost );
}
AJarvis::~AJarvis() {
	lua_close( __L );
	delete __pHost;
}


/// /// ///
///
/// Класс интерпритатора
LuaEmbInterpreter::LuaEmbInterpreter( ) {
	_L = luaL_newstate( );
	luaL_openlibs( _L );
}
LuaEmbInterpreter::~LuaEmbInterpreter( ) {
	lua_close( _L );
}
LocalStr LuaEmbInterpreter::getStringFrom( LocalStr nameScript, LocalStr nameVar ) {
	LocalStr result;
	//
	luaL_dofile( _L, nameScript.c_str( ) );

	// читаем настройки из скрипта
	lua_getglobal( _L, nameVar.c_str() );

	// читаем
	result = lua_tostring( _L, -1 );
	// проверку бы сделать на ошибки

	return result;
}
std::vector< LocalStr > LuaEmbInterpreter::getArrayStringFrom( LocalStr nameScript, LocalStr nameArray ) {
	vector< LocalStr > result;

	// запускае скрипт
	luaL_dofile( _L, nameScript.c_str( ) );

	// попытки счиатть таблицу
	lua_getglobal( _L, nameArray.c_str( ) );
	lua_pushnil( _L );
	while(lua_next( _L, -2 ) ) { 
		if(lua_isstring( _L, -1 ) ) {
			string i = lua_tostring( _L, -1 );
			result.push_back( i );
		}
		lua_pop( _L, 1 );
	}
	lua_pop( _L, 1 );

	//
	return result;
}


double LuaEmbInterpreter::getDoubleFrom( LocalStr nameScript, LocalStr nameVar ) {
	double result;
	//
	luaL_dofile( _L, nameScript.c_str( ) );

	// читаем настройки из скрипта
	lua_getglobal( _L, nameVar.c_str() );

	// читаем
	result = lua_tonumber( _L, -1 );
	// проверку бы сделать на ошибки

	return result;
}

std::vector< double > LuaEmbInterpreter::getArrayDoubleFrom( LocalStr nameScript, LocalStr nameArray ) {
	vector< double > result;

	// запускае скрипт
	luaL_dofile( _L, nameScript.c_str( ) );

	// попытки счиатть таблицу
	lua_getglobal( _L, nameArray.c_str( ) );
	lua_pushnil( _L );
	while(lua_next( _L, -2 ) ) { // <== here is your mistake
		if(lua_isnumber( _L, -1 ) ) {
			double i = (double )lua_tonumber( _L, -1 );
			result.push_back( i );
			//use number
		}
		lua_pop( _L, 1 );
	}
	lua_pop( _L, 1 );

	//
	return result;
}
#endif