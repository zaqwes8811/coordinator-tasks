//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <stdio.h>
#include <winsock.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
/**
    1. ����� � ������ ������ ����� ������������ �����
    2. ������� ����
    3. ��������� ������
*/
struct sockaddr_in addr;
char *cIP = new char[20*4];   // ����� � ��������
char *cMask = new char[20*4];   // ����� � ��������
*cIP = 0;
*cMask = 0;
unsigned long ulIP;   // int
unsigned long ulMask;   // ����� � ��������
// ����������� �����
AnsiString AIP = Edit1->Text;
cIP = AIP.c_str();
addr.sin_addr.s_addr = inet_addr(cIP);
ulIP = ntohl(addr.sin_addr.s_addr );
ulMask =  ulIP & 0xFF000000;
if((ulMask & 0x80000000) == 0x00000000)  // a
    cMask = "255.0.0.0";
else if((ulMask & 0xC0000000) == 0x80000000) // b
        cMask = "255.255.0.0";
        else if((ulMask & 0xE0000000) == 0xC0000000)  // c
            cMask = "255.255.255.0";
            else
                return;
// ������ � ����
    FILE* pf_write;
    pf_write = fopen("netset.txt", "wt"); // ���������
    fprintf(pf_write,"%s", cIP);  // ������ ���������
    fprintf(pf_write,"%s", "\n");  // ������ ���������
    fprintf(pf_write,"%s", cMask);  // ������ ���������
    // ���������� ������� ������ (�������� ������ �� ��� ��������)
    delete [] cIP;
    delete [] cMask;
    fclose(pf_write) ;

// ������ �������
HANDLE Handl;
    ShellExecute(Handl, "open", "net.vbs", NULL, NULL, SW_SHOW);        
}
//---------------------------------------------------------------------------
