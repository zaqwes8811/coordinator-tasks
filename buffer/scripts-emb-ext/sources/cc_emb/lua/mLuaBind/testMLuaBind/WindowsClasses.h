/*
	Цель :
		Менять содержимое класса извне, запуская интерпр. внутри класса.
		При этом объект не должнен копироваться. Сам класс нельзя обернуть 
			в смарт-указатель. Луа не должна удалять никакие объекты.

	Важно :
		Обертки вызываются через глобальное простр. имен. Прямо передать 
			сходу не вышло.

*/
#pragma once

#include <mluabind/mluabind.h>
#include <Loki/SmartPtr.h>
//#include "mJarvis.h"

#include <iostream>
#include <memory>

void trace ( std::string msg );

class Windows {
	public :
		Windows( );
		~Windows( );
		// 
		void Run();
		std::string getHello();
		void setCore( std::string str );
	private :
		std::string _getString();
		std::string _hello;

		// for mt runner : std::auto_ptr< AJarvis > jarvis(
};

class WindowsWrapper {
	public :
		explicit WindowsWrapper( Windows* pWindows );
		explicit WindowsWrapper( );
		~WindowsWrapper();

		std::string getHello();
		void setCore( std::string str );

	private : 
		Windows* _pw;
};

// нужно для интерпритатора
typedef Loki::SmartPtr< WindowsWrapper > PtrWndWrapper;
PtrWndWrapper getMgr( );
void setMgr( PtrWndWrapper sPtr );
extern PtrWndWrapper g_loki_wind_wr;

// интерпритатор
class mJarvisWindows// : public EmbInterpreters::AJarvis 
{
	public :
    explicit mJarvisWindows() {}  //: AJarvis() {}
		EmbInterpreters::ErrCode exhibit();
		EmbInterpreters::ErrCode runScriptInCurrentThread( EmbInterpreters::Str fname );
		EmbInterpreters::ErrCode runScriptInNewThread( EmbInterpreters::Str fname );
};
