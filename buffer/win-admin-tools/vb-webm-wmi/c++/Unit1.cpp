//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma link "wbemuuid.lib"   // моежт не арботат
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
 CoInitialize(NULL);

 if(CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0) != S_OK)

  {

    ShowMessage("Неполучилось:(!");

    Application->Terminate();

  }
}
//---------------------------------------------------------------------------
void TForm1::GetWmiInfo(TStrings *lpList, WideString wsClass)

{
 IWbemLocator *pWbemLocator = NULL;
 if(CoCreateInstance(CLSID_WbemAdministrativeLocator, NULL, CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER, IID_IUnknown, (void**)&pWbemLocator) == S_OK)
  {
    IWbemServices *pWbemServices = NULL;
    WideString wsNamespace = (L"root\\cimv2");
    if(pWbemLocator->ConnectServer(wsNamespace, NULL, NULL, NULL, 0, NULL, NULL, &pWbemServices) == S_OK)
     {
       IEnumWbemClassObject *pEnumClassObject = NULL;
       WideString wsWQL=L"WQL", wsQuery=WideString(L"Select * from ")+wsClass;
       if(pWbemServices->ExecQuery(wsWQL, wsQuery, WBEM_FLAG_RETURN_IMMEDIATELY,NULL, &pEnumClassObject) == S_OK)
        {
          IWbemClassObject *pClassObject = NULL;
          ULONG uCount = 1, uReturned;
          if(pEnumClassObject->Reset() == S_OK)
           {
             int iEnumIdx = 0;
             while(pEnumClassObject->Next(WBEM_INFINITE, uCount, &pClassObject, &uReturned) == S_OK)
              {
                lpList->Add("---------------- ["+IntToStr(iEnumIdx)+"] -----------------");
                SAFEARRAY *pvNames = NULL;
                if(pClassObject->GetNames(NULL, WBEM_FLAG_ALWAYS | WBEM_MASK_CONDITION_ORIGIN, NULL, &pvNames) == S_OK)
                  {
                   long vbl, vbu;
                   SafeArrayGetLBound(pvNames, 1, &vbl);
                   SafeArrayGetUBound(pvNames, 1, &vbu);
                   for(long idx=vbl; idx<=vbu; idx++)
                    {
                      long aidx = idx;
                      wchar_t *wsName = 0;
                      VARIANT vValue;
                      VariantInit(&vValue);
                      SafeArrayGetElement(pvNames, &aidx, &wsName);
                      BSTR bs = SysAllocString(wsName);
                      HRESULT hRes = pClassObject->Get(bs, 0, &vValue, NULL, 0);
                      SysFreeString(bs);
                      if(hRes == S_OK)
                        {
                         AnsiString s;
                         Variant v = *(Variant*)&vValue;
                         if(v.IsArray())
                          {
                            for(int i=v.ArrayLowBound(); i<=v.ArrayHighBound(); i++)
                             {
                               Variant a = v.GetElement(i);
                               if(!s.IsEmpty())
                                 s+=", ";
                               s+=VarToStr(a);
                             }
                          }
                         else
                          {
                            s = VarToStr(v);
                          }
                         lpList->Add(AnsiString(wsName)+"="+s);
                       }
                      VariantClear(&vValue);
                      SysFreeString(wsName);
                    }
                 }
                if(pvNames)SafeArrayDestroy(pvNames);
                iEnumIdx++;
              }
           }
          if(pClassObject)pClassObject->Release();
        }
       if(pEnumClassObject)pEnumClassObject->Release();
     }
    if(pWbemServices)pWbemServices->Release();
  }
 if(pWbemLocator)pWbemLocator->Release();
}
//__fastcall TForm1::~TForm1()

//{

//

//}
void __fastcall TForm1::Button1Click(TObject *Sender)
{
GetWmiInfo(Memo1->Lines,   "Win32_BIOS");    
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
  CoUninitialize();   //При закрытии формы закрываем работу с WMI
  Application->Terminate();
}
//---------------------------------------------------------------------------
