//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("MetroLib.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("MetroData.cpp");
USEUNIT("MetroChart.cpp");
USEUNIT("MetroParams.cpp");
USEUNIT("MetroCaption.cpp");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
  return 1;
}
//---------------------------------------------------------------------------


