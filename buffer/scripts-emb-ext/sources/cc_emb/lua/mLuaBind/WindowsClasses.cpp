#include "WindowsClasses.h"
using namespace mluabind;
using namespace EmbInterpreters;
using namespace std;

void Windows::Run( ) {

	// передаем указатель на себя в глоб. простр. имен
	PtrWndWrapper loki_wind_wr( new WindowsWrapper( this ) );

	// передаем в глобальное пространство имен
	::setMgr( loki_wind_wr );

	// вот интерпритатор
	std::auto_ptr< AJarvis > jarvis( new mJarvisWindows( ) );
	
	// экспонируем классы
	jarvis->exhibit();

	// запускаем скрипт
	string fname = "config.lua";
	jarvis->runScriptInCurrentThread( fname );

	// работало - ссылку не удаляет
	trace( this->getHello( ) );
}

// прочие реализации
Windows::Windows( ) { _hello = "hello";	trace( "constr dflt" ); }

std::string Windows::getHello( ) { return this->_getString( ); };

Windows::~Windows ( ) { trace( "destr dflt" ); }

void Windows::setCore( std::string str ) { _hello = str; }

std::string Windows::_getString( ) { return _hello; }

// обертка класса окна
WindowsWrapper::WindowsWrapper( Windows* pWindows ) { 
	trace( "wr. constr. extend" ); _pw = pWindows; 
}

WindowsWrapper::WindowsWrapper( ) { trace( "wr. constr. dflt" ); }

WindowsWrapper::~WindowsWrapper( ) { trace( "wr. destr." ); }

std::string WindowsWrapper::getHello( ) { return _pw->getHello( ); }

void WindowsWrapper::setCore( std::string str ) { _pw->setCore( str ); }

void trace( std::string msg ) {
	std::cout <<  msg <<  std::endl;
}

/// Встраивание интерпритатора
// Глобальные объекты - костыль, но пока хватает
PtrWndWrapper g_loki_wind_wr( new WindowsWrapper( ) );

PtrWndWrapper getMgr( ) { 
	trace( "get ptr ");
	return g_loki_wind_wr; 
}

void setMgr( PtrWndWrapper sPtr ) { g_loki_wind_wr = sPtr; }

// реализация самого класса интерпретатора
ErrCode mJarvisWindows::runScriptInCurrentThread( EmbInterpreters::Str fname ) {
	// запускаем скрипт
	luaL_dofile( __L, fname.c_str() );
}

ErrCode mJarvisWindows::exhibit() {
	// Экспонируем обертку себя
	// Это тот случай когда нельзя завернуть указатель в смарт обертку - 
	//   например класс окна MFC
	__pHost->Insert( 
		Class< WindowsWrapper >( "WindowsWrapper" )
			.Constructor( )
			.Method( "getHello", &WindowsWrapper::getHello )
			.Method( "setCore", &WindowsWrapper::setCore )
		 );

	// Экспонируем смарт-указатель
	// Можно попробовать передать не смарт, но при закрытии инт. сборщик
	//   мусора в луа вызовет деструктор и может быть двойное удаление. 
	//   Может и не беда, но лучше RAII, а так получается, что объект создан
	//   в си часте, а удален в луа
	__pHost->Insert( 
		Class< PtrWndWrapper  >( )
			.SmartPtr< WindowsWrapper >( )
	 );

	// получатель указатель из глобального простр. имен
	__pHost->Insert( Function( "getMgr", &::getMgr, Adopt( -1 ) ) );
}

EmbInterpreters::ErrCode runScriptInNewThread( EmbInterpreters::Str fname ) {

}
