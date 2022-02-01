// MyWmi.cpp : main project file.
 
#include "stdafx.h"
using namespace System;
using namespace System::Diagnostics;
using namespace System::Security;
using namespace System::Management;
using namespace System::ComponentModel;
using namespace System::IO;
using namespace System::Threading;
// just a normal function
//ref class ethernetManagment {


//}
int GetNumAvailableConnections()
{
  // Query for the list of network adapters on the machine
  // Note for more detail on this type you can search 
  // for "Win32_NetworkAdapterConfiguration"
  // http://msdn.microsoft.com/en-us/library/aa394217(VS.85).aspx
  SelectQuery^ NAQuery = gcnew SelectQuery("select * from Win32_NetworkAdapterConfiguration");
  ManagementObjectSearcher^ NASearcher = gcnew ManagementObjectSearcher(NAQuery);
  int availableConnections = 0;
  try {
    // Loop through each adapter returned from the query
    for each (ManagementObject^ enumerate in NASearcher->Get()) {
      // Find out if IP is enabled on this adapter
      bool  IPEnabled = (bool)enumerate["IPEnabled"];
      // Get the adapter description
      Console::WriteLine((String^)enumerate["Caption"]);
      // If the IP is enabled check for non-zero IP
      if(IPEnabled) {
        array<String^>^ IPAddress = (array<String^>^)enumerate["IPAddress"];
		array<String^>^ DefaultIPGateway = (array<String^>^)enumerate["DefaultIPGateway"];
		array<String^>^ IPSubnet = (array<String^>^)enumerate["IPSubnet"];
        Console::WriteLine("IP Address: " + IPAddress[0]);
		Console::WriteLine("DefaultIPGateway: " + DefaultIPGateway[0]);
		Console::WriteLine("IPSubnet: " + IPSubnet[0]);
		/// установка
		
		/// подсчет
        if(IPAddress[0] != "0.0.0.0")  {
          availableConnections++;
		  // читаем старые
          array<String^>^ IPAddress =(array<String^>^)(enumerate->GetPropertyValue("IPAddress"));//gcnew String("172.23.132.14");
          array<String^>^ IPSubnet = (array<String^>^)(enumerate->GetPropertyValue("IPSubnet"));
          array<String^>^ DefaultIPGateway = (array<String^>^)(enumerate->GetPropertyValue("DefaultIPGateway"));
		  array<System::UInt16>^ Metric = (array<System::UInt16>^)(enumerate->GetPropertyValue("GatewayCostMetric"));
		  IPAddress[0] = "172.23.132.223";
          IPSubnet[0] = "255.255.224.0";
		  DefaultIPGateway[0] = "172.23.128.254";
		  Metric[0] = 1;
          ManagementBaseObject^ objNewIP = (ManagementBaseObject ^)(enumerate->GetMethodParameters("EnableStatic"));
		  ManagementBaseObject^ objGwIP = (ManagementBaseObject ^)(enumerate->GetMethodParameters("SetGateways")); 
          // шлюз
		  objGwIP->SetPropertyValue("DefaultIPGateway", DefaultIPGateway);
		  objGwIP->SetPropertyValue("GatewayCostMetric", Metric);
		  // сеть
		  objNewIP->SetPropertyValue("IPAddress",IPAddress);
          objNewIP->SetPropertyValue("SubnetMask",IPSubnet);
          ManagementBaseObject^ objIP = ((ManagementObject ^)enumerate)->InvokeMethod("EnableStatic",objNewIP, nullptr);
		  Console::WriteLine("Itogo: "+objIP["returnValue"]);
		  ManagementBaseObject^ objIPGw = ((ManagementObject ^)enumerate)->InvokeMethod("SetGateways",objGwIP, nullptr);
		  Console::WriteLine("Itogo: "+objIPGw["returnValue"]);

        }
      }
      Console::WriteLine();
    }
  }
  catch(Exception^ err) {
    Console::WriteLine("You've got some error: " + err->ToString());
  }
  return availableConnections;
}

/// <summary>
/// This sample demonstrates the use of System.Managment
/// to detect whether or not one or more valid network
/// connections is associated with the machine.
/// </summary>
[STAThread]
int main(array<System::String ^> ^args)
{
  // Check to see if there is one or more connections
  // assigned to this machine (based on IP address)
  int numConnections = GetNumAvailableConnections();
  if(numConnections > 0) {       
	if(numConnections == 1) {
      Console::WriteLine("1 connection on this machine with ");
      Console::WriteLine("at least 1 valid IP address assigned");
    } else {
      Console::WriteLine(numConnections + " connections on this ");
      Console::WriteLine("machine with at least 1 valid IP address");
    }
  } else {
      Console::WriteLine("No cards found with a valid IP address");
  }
  //Thread::Sleep(4000);
  return 0;
}