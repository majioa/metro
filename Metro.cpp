//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("Metro.res");
USEFORM("MetroUnit.cpp", MetroForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
    Application->Initialize();
    Application->Title = "Metro";
    Application->CreateForm(__classid(TMetroForm), &MetroForm);
    Application->Run();
  }
  catch (Exception &exception)
  {
    Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
