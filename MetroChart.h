//---------------------------------------------------------------------------

#ifndef MetroChartH
#define MetroChartH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Math.hpp>

//#include "c:\work\projects\lib\AdvancedText.h"
#include <math.h>
//---------------------------------------------------------------------------
class PACKAGE TMetroChart;

#include "MetroData.h"
#include "MetroParams.h"

const long double Pi = 3.1415926535897932384626433832795;

struct TFloatPoint
{
  TFloatPoint() {}
  TFloatPoint(int _x, int _y) : x(_x), y(_y) {}
  TFloatPoint(POINT& pt)
  {
    x = pt.x;
    y = pt.y;
  }
  operator POINT() const
  {
    POINT pt;
    pt.x = x;
    pt.y = y;
    return pt;
  }
  float  x;
  float  y;
};

class PACKAGE TMetroChart : public TGraphicControl
{
private:
  TMetroData * FMetroData;
  Graphics::TBitmap * FBkgndBmp;
  Graphics::TBitmap * FChartBmp;
  Graphics::TBitmap * FLegendBmp;
  TFloatPoint FCoeff;
  bool FShow;
  int FStatusSize;
  TMetroParams *FMetroParams;
  bool FLegend;

  void __fastcall SetMetroData(TMetroData * value);
  void __fastcall LoadComplete(TObject * Sender);
  void __fastcall SetCoeff(TFloatPoint value);
  void __fastcall MapChartData();
  void __fastcall PaintStation(Graphics::TBitmap *bmp, TPoint p, TColor color);
  void __fastcall PaintTriangle(Graphics::TBitmap *bmp, int lx, int ly, int x1, int y1, bool fl);
  void __fastcall PaintTrain(Graphics::TBitmap *bmp, TRect r, TColor color, TPassageDirection dir);
  void __fastcall PaintWalk(Graphics::TBitmap *bmp, TRect r, int angle);
  void __fastcall PaintRect(Graphics::TBitmap *bmp, TRect r, int len, int shift, int blank, TColor color, TShowRectMode mode, TPassageDirection dir);
  TRect __fastcall GetCalcRect(TRect rect, int len, long double sin, long double cos);
  void __fastcall PaintStationName(Graphics::TBitmap *bmp, TPoint p, unsigned int a, AnsiString name);
  TRect __fastcall Rect(int left, int top, int right, int bottom, TFloatPoint coeff);
  TRect __fastcall ChordRect(TRect r, int angle);
  void __fastcall SetMetroParams(TMetroParams * value);
  void __fastcall MapLegendData();
  void __fastcall PaintLineName(Graphics::TBitmap *bmp, TPoint p, TNameAlign na, AnsiString caption);
  void __fastcall PaintTrialTrain(Graphics::TBitmap *bmp, TRect r, TColor color);
  void __fastcall SetLegend(bool value);
  void __fastcall CutFwdEdge(Graphics::TBitmap *bmp, TRect r, int shift, int blank, int x1, int y1);
  void __fastcall CutBwdEdge(Graphics::TBitmap *bmp, TRect r, int shift, int blank, int x1, int y1);
protected:
public:
  __fastcall TMetroChart(TComponent* Owner);
  __fastcall ~TMetroChart();
  void __fastcall Paint();
  __property TFloatPoint Coeff  = { read=FCoeff, write=SetCoeff };
  __property int StatusSize  = { read=FStatusSize };
__published:
  __property TMetroParams * MetroParams  = { read=FMetroParams, write=SetMetroParams };
  __property TMetroData * MetroData  = { read=FMetroData, write=SetMetroData };
  __property bool Legend  = { read=FLegend, write=SetLegend };
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
