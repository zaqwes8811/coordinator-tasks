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
    1. Найти к какому классу сетей пренадлежить адрес
    2. Сделать файл
    3. запустить скрипт
*/
struct sockaddr_in addr;
char *cIP = new char[20*4];   // берем с запастом
char *cMask = new char[20*4];   // берем с запастом
*cIP = 0;
*cMask = 0;
unsigned long ulIP;   // int
unsigned long ulMask;   // берем с запастом
// опредеелние класс
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
// запись в файл
    FILE* pf_write;
    pf_write = fopen("netset.txt", "wt"); // открываем
    fprintf(pf_write,"%s", cIP);  // сперва заголовок
    fprintf(pf_write,"%s", "\n");  // сперва заголовок
    fprintf(pf_write,"%s", cMask);  // сперва заголовок
    // особождаем занятую память (интерсно долгие ли это операции)
    delete [] cIP;
    delete [] cMask;
    fclose(pf_write) ;

// запуск скрипта
HANDLE Handl;
    ShellExecute(Handl, "open", "net.vbs", NULL, NULL, SW_SHOW);        
}
//---------------------------------------------------------------------------
