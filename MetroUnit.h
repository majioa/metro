//---------------------------------------------------------------------------

#ifndef MetroUnitH
#define MetroUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
#include "MetroParams.h"
#include "MetroData.h"
#include "MetroChart.h"
#include "MetroCaption.h"
//---------------------------------------------------------------------------
class TMetroForm : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TSpeedButton *Load;
  TSpeedButton *Exit;
  TOpenDialog *OpenMetro;
  TStatusBar *StatusBar;
  TMetroChart *MetroChart;
  TMetroParams *MetroParams;
  TMetroData *MetroData;
  void __fastcall LoadClick(TObject *Sender);
  void __fastcall ExitClick(TObject *Sender);
  void __fastcall MetroDataLoadComplete(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
  int *FBuf;
public:		// User declarations
        __fastcall TMetroForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMetroForm *MetroForm;
//---------------------------------------------------------------------------
#endif
