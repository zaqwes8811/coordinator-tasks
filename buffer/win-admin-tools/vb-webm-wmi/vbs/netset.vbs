' ? ��� �������� � ������ ��������(��. ������� Array())
'arglist = "172.23.132.223"
' �������� ������
arrIPAddress 	= 	Array("  172.23.132.223  ")		' IP
arrSubnetMask 	= 	Array("  255.255.248.0   ")		' �����
arrGateway 		= 	Array("  172.23.128.254  ")		' ����
arrMetric 		= 	Array(1)					' ������� �����

'' ����� ������� ��� ������� � ���������� TCP/IP ���������� ���������� � ������� WMI
strComputer = "." 
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")
' ���� ����������
Set colNetAdapters = objWMIService.ExecQuery("Select * from Win32_NetworkAdapterConfiguration")

' ���������������� ���������
For Each objNetAdapter in colNetAdapters
	errEnableStatic = objNetAdapter.EnableStatic(arrIPAddress, arrSubnetMask) ' IP � �����(��� ���� ���������)
	errGateway = objNetAdapter.SetGateways(arrGateway, arrMetric)		' ���� �� ���������(���� ��� ����)
Next 


