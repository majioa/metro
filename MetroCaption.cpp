//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MetroCaption.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMetroCaption *)
{
  new TMetroCaption(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroCaption::TMetroCaption(TComponent* Owner)
  : TGraphicControl(Owner)
{
  FBmp = new Graphics::TBitmap();
}
//---------------------------------------------------------------------------
namespace Metrocaption
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMetroCaption)};
     RegisterComponents("Metro", classes, 0);
  }
}
//---------------------------------------------------------------------------


__fastcall TMetroCaption::~TMetroCaption()
{
  delete FBmp;
}

void __fastcall TMetroCaption::SetMetroData(TMetroData * value)
{
  if(FMetroData == value)
    return;
  FMetroData = value;
}

void __fastcall TMetroCaption::Paint()
{
  if(!Caption.IsEmpty())
    {
    TRect rect = ::Rect(0,0,FBmp->Width,FBmp->Height);
    Canvas->BrushCopy(rect,FBmp,rect,FMetroData->Color);
    }
}

void __fastcall TMetroCaption::SetCaptionSize(int value)
{
  if(FCaptionSize == value)
    return;
  FCaptionSize = value;
}




void __fastcall TMetroCaption::MapCaptionData()
{
  //show caption if it exist
  if(!Caption.IsEmpty())
    {
    int k;
    AnsiString str = Caption, str1;
    do
      {
      do
        {
        k = str.Pos(" ");
        if(k)
          {
          str1 = str.SubString(1, k - 1);
          str = str.SubString(k + 1, str.Length() - k).TrimLeft();
          }
        else
          str1 = str;
        } while (FBmp->Canvas->TextWidth(str1));
      }  while(false);
    }
}

void __fastcall TMetroCaption::SetMetroCaption(AnsiString value)
{

  Text = Filter(value);
}
AnsiString __fastcall TMetroCaption::GetMetroCaption()
{
  return Text;
}

AnsiString __fastcall TMetroCaption::Filter(AnsiString value)
{
  //TODO: Add your source code here
  return "";
}

void __fastcall TMetroCaption::SetBorderWidth(int value)
{
  if(FBorderWidth != value)
    return;
  FBorderWidth = value;
}

void __fastcall TMetroCaption::SetBorderColor(TColor value)
{
  if(FBorderColor != value)
    return;
  FBorderColor = value;
}

void __fastcall TMetroCaption::SetTopShift(int value)
{
  if(FTopShift != value)
    return;
  FTopShift = value;
}

void __fastcall TMetroCaption::SetLeftShift(int value)
{
  if(FLeftShift != value)
    return;
  FLeftShift = value;
}

void __fastcall TMetroCaption::SetBottomShift(int value)
{
  if(FBottomShift != value)
    return;
  FBottomShift = value;
}

void __fastcall TMetroCaption::SetRightShift(int value)
{
  if(FRightShift == value)
    return;
  FRightShift = value;
}

