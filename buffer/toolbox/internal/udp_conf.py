#-*- coding:utf-8 -*-
from pyDbg.doColoredConsole import co
import simpleDataTypesConvertors.IntTypeConvertors as tc
''' '''
def TxProcessorOK12( data ):
	for at in range( len( data ) ):
		msg = tc.byte2hex( ord( data[at] ) )
		# фильтруем вывод
		if at == 5:
			co.printW( msg )
		else :
			co.printN( msg )
''' '''
def RxFilterOK12( data ):
	if ord(data[4] ) == 1:# or ord(data[4] ) == 4:
		return True
	else :
		return False
		
''' третий и четверный оконечные '''
def RxFilterOK34( data ):
	if ord(data[4] ) == 4:# or ord(data[4] ) == 4:
		return True
	else :
		return False

''' данные со всех оконечных '''
def RxFilterOK1234( data ):
	if ord(data[4] ) == 4 or ord(data[4] ) == 1:
		return True
	else :
		return False
		
def TxFilterOK12( data ):
	if ord(data[4] ) == 3:# or ord(data[4] ) == 4:
		return True
	else :
		return False
		
''' '''
def RxProcessorOK12( data ):
	for at in range( len( data )-4 ):
		msg = tc.byte2hex( ord( data[at] ) )
		# фильтруем вывод
		if at < 5:
			if at == 4:
				pass #co.printW( msg )
				#co.printN ( '\n' )
			else :
				pass #co.printN( msg )
		else :
			# форматирование
			co.printN( msg )
			if (at-4)%13 == 0 and  (at-4) < 14:
				co.printN ( '\nRx >> ' )

TxFilter = TxFilterOK12
TxProcessor = TxProcessorOK12
RxFilter = RxFilterOK12#RxFilterOK34#RxFilterOK12
RxProcessor = RxProcessorOK12

def printRx( data ):
	# downsampler
	#if(i%step == 0):
	# фильтрация
	if len( data ) > 5:
		if( RxFilter( data ) ):
			co.printW ( '\nRx >> ' )
			RxProcessor( data )
			#co.printN ( '\n' )
	# следующий шаг
	#i += 1
	
def printTx( data ):
	# downsampler
	#if(i%step == 0):
	# фильтрация
	if( TxFilter( data ) ):
		co.printW ( '\nTx >> ' )
		TxProcessor( data )
		co.printN ( '\n' )
	# следующий шаг
	#i += 1