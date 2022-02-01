strComputer = "."
Set objWMIService = GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set colSoftware = objWMIService.ExecQuery ("Select * from Win32_Product")'
Set colSettings = objWMIService.ExecQuery ("Select * from Win32_OperatingSystem")

WScript.Echo "Installed Software List and OS Information"
WScript.Echo "=========================================="
Wscript.Echo ""

'Computer Information'
Wscript.Echo "COMPUTER INFORMATION"
Wscript.Echo "===================="
Wscript.Echo "Computer Name: " & strComputer
For Each objOperatingSystem in colSettings
'Crop Operating system name for clean name'
Result = InStr(objOperatingSystem.Name, "|")
Result = Result -1
MyString = Left(objOperatingSystem.Name,Result)
'Echo stuff
Wscript.Echo "Operating System: " & MyString
Wscript.Echo "Service Pack: " & objOperatingSystem.ServicePackMajorVersion & "." & objOperatingSystem.ServicePackMinorVersion
Wscript.Echo "Installed In: " & objOperatingSystem.WindowsDirectory
Wscript.Echo ""
Next

'Software Information'
Wscript.Echo "SOFTWARE LIST"
Wscript.Echo "============="
For Each objSoftware in colSoftware
wscript.echo objSoftware.Caption
Next 