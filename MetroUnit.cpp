//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MetroUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
/*#pragma link "MetroData"
#pragma link "MetroChart"
#pragma link "MetroParams"
#pragma link "MetroCaption"*/
#pragma resource "*.dfm"
TMetroForm *MetroForm;
//---------------------------------------------------------------------------
__fastcall TMetroForm::TMetroForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMetroForm::LoadClick(TObject *Sender)
{
  if(!OpenMetro->Execute())
    return;
  MetroData->LoadMetroFile(OpenMetro->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TMetroForm::ExitClick(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TMetroForm::MetroDataLoadComplete(TObject *Sender)
{
  Application->Title = MetroData->Caption;
  Caption = MetroData->Caption;          
  StatusBar->Panels->Items[0]->Text = "File " + ExtractFileName(OpenMetro->FileName) + " is loaded successfully";
}
//---------------------------------------------------------------------------
void __fastcall TMetroForm::FormCreate(TObject *Sender)
{
  if(FileExists("Metro.ini"))
    MetroParams->LoadIniFile("Metro.ini");
  else
    StatusBar->Panels->Items[0]->Text = "Config file Metro.ini is not found. Default settings are used";
}
//---------------------------------------------------------------------------

