strComputer = "." 

Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\CIMV2")

Set colItems = objWMIService.ExecQuery("SELECT * FROM Win32_NetworkAdapterConfiguration",,48) 

For Each objItem in colItems 
    Wscript.Echo "-----------------------------------"
    Wscript.Echo "Win32_NetworkAdapterConfiguration instance"
    Wscript.Echo "-----------------------------------"
    If isNull(objItem.IPAddress) Then
        Wscript.Echo "IPAddress: "
    Else
        Wscript.Echo "IPAddress: " & Join(objItem.IPAddress, ",")
    End If
	
	Wscript.Echo "IPAddress: " & Join(objItem.DefaultIPGateway, ",")
Next
