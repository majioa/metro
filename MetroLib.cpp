//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("MetroLib.res");
USEPACKAGE("vcl50.bpi");
USEPACKAGE("pskrlib.bpi");
USEUNIT("MetroChart.cpp");
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
