//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MetroInfo.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMetroInfo *)
{
  new TMetroInfo(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroInfo::TMetroInfo(TComponent* Owner)
  : TGraphicControl(Owner)
{
}
//---------------------------------------------------------------------------
namespace Metroinfo
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMetroInfo)};
     RegisterComponents("Metro", classes, 0);
  }
}
//---------------------------------------------------------------------------
 