' объект-файл
Set FSO = CreateObject("Scripting.FileSystemObject")
	'открываем файл для чтения
    Set t_file = FSO.OpenTextFile("netset.txt", 1)
	' читаме настройки
    strIP = t_file.ReadLine
    strMask = t_file.ReadLine
    t_file.Close

' некоторые преобразования
strIPAddress = Array(strIP)
strSubnetMask = Array(strMask)
'strGateway = Array("172.23.128.254")
'strGatewayMetric = Array(1)
'далее работа с WMI
'If strIP <> "" Then
strComputer = "."
Set objWMIService = GetObject("winmgmts:" _
    & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")

Set colNetAdapters = objWMIService.ExecQuery _
    ("Select * from Win32_NetworkAdapterConfiguration where IPEnabled=TRUE")
For Each objNetAdapter in colNetAdapters
    errEnable = objNetAdapter.EnableStatic(strIPAddress, strSubnetMask)
    'errGateways = objNetAdapter.SetGateways(strGateway, strGatewaymetric)
    If errEnable = 0 Then
       WScript.Echo "Настроки взяты из файла netset.txt и установлены следующими: " + Chr(13) & Chr(10) + "IP Adress: " & strIP + Chr(13) & Chr(10) +"SubNetMask: " & strMask
	Else
        WScript.Echo "Настройки изменить не удалось."
    End If
Next
'End If
