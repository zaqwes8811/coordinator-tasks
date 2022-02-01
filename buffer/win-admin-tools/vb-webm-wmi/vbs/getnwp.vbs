' ? как изменить у одного адаптера(см. функции Array())
'arglist = "172.23.132.223"
' исходные данные
arrIPAddress 	= 	Array("  172.16.100.88  ")		' IP
arrSubnetMask 	= 	Array("  255.255.0.0   ")		' Маска
arrGateway 		= 	Array("  172.16.11.77  ")		' Шлюз
arrMetric 		= 	Array(1)					' метрика шлюза

'' нужно сделать для доступа к установкам TCP/IP локального компьютера с помощью WMI
strComputer = "." 
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")
' сбор информации
Set colNetAdapters = objWMIService.ExecQuery("Select * from Win32_NetworkAdapterConfiguration")

' непосредственное изменение
For Each objNetAdapter in colNetAdapters
	errEnableStatic = objNetAdapter.EnableStatic(arrIPAddress, arrSubnetMask) ' IP и маски(для всех адаптеров)
	errGateway = objNetAdapter.SetGateways(arrGateway, arrMetric)		' шлюз по умолчанию(тоже для всех)
Next 


