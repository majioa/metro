//---------------------------------------------------------------------------

#ifndef MetroCaptionH
#define MetroCaptionH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include "MetroData.h"

class PACKAGE TMetroCaption : public TGraphicControl
{
private:
  Graphics::TBitmap * FBmp;
  TMetroData *FMetroData;
  int FCaptionSize;
  TStringList * FWords;
  int FBorderWidth;
  TColor FBorderColor;
  int FTopShift;
  int FLeftShift;
  int FBottomShift;
  int FRightShift;

  void __fastcall SetCaptionSize(int value);
  void __fastcall SetMetroData(TMetroData * value);
  void __fastcall Paint();
  void __fastcall MapCaptionData();
  void __fastcall SetMetroCaption(AnsiString value);
  AnsiString __fastcall GetMetroCaption();
  AnsiString __fastcall Filter(AnsiString value);
  void __fastcall SetBorderWidth(int value);
  void __fastcall SetBorderColor(TColor value);
  void __fastcall SetTopShift(int value);
  void __fastcall SetLeftShift(int value);
  void __fastcall SetBottomShift(int value);
  void __fastcall SetRightShift(int value);
protected:
public:
  __fastcall TMetroCaption(TComponent* Owner);
  __fastcall ~TMetroCaption();
  __property AnsiString MetroCaption  = { read=GetMetroCaption, write=SetMetroCaption };
  __property int BorderWidth  = { read=FBorderWidth, write=SetBorderWidth };
  __property TColor BorderColor  = { read=FBorderColor, write=SetBorderColor };
  __property int TopShift  = { read=FTopShift, write=SetTopShift };
  __property int LeftShift  = { read=FLeftShift, write=SetLeftShift };
  __property int BottomShift  = { read=FBottomShift, write=SetBottomShift };
  __property int RightShift  = { read=FRightShift, write=SetRightShift };
__published:
  __property int CaptionSize  = { read=FCaptionSize, write=SetCaptionSize };
  __property TMetroData * MetroData  = { read=FMetroData, write=SetMetroData };
  __property Color;
  __property Constraints;
  __property Height;
  __property Hint;
  __property ShowHint;
  __property Left;
  __property Top;
  __property Name;
  __property Visible;
  __property Enabled;
  __property Anchors;
  __property Align;
  __property Action;
  __property BiDiMode;
  //events
  __property OnCanResize;
  __property OnClick;
  __property OnConstrainedResize;
  __property OnDblClick;
  __property OnDragDrop;
  __property OnDragOver;
  __property OnEndDock;
  __property OnEndDrag;
  __property OnMouseDown;
  __property OnMouseMove;
  __property OnMouseUp;
  __property OnStartDock;
  __property OnStartDrag;
  __property OnResize;
};
//---------------------------------------------------------------------------
#endif
 