//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MetroChart.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMetroChart *)
{
  new TMetroChart(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroChart::TMetroChart(TComponent* Owner)
  : TGraphicControl(Owner)
{
  FBkgndBmp = new Graphics::TBitmap();
  FChartBmp = new Graphics::TBitmap();
  FLegendBmp = new Graphics::TBitmap();
  FCoeff = TFloatPoint(1,1);
}
//---------------------------------------------------------------------------
__fastcall TMetroChart::~TMetroChart()
{
  delete FLegendBmp;
  delete FChartBmp;
  delete FBkgndBmp;
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::Paint()
{
  if(!FMetroData || !FShow)
    return;
  if(!FBkgndBmp->Empty)
    Canvas->BrushCopy(ClientRect,FBkgndBmp,::Rect(0,0,FBkgndBmp->Width,FBkgndBmp->Height),clBackground);
  else
    {
    Canvas->Pen->Color = FMetroData->Color;
    Canvas->Brush->Color = FMetroData->Color;
    Canvas->Brush->Style = bsSolid;
    Canvas->Rectangle(ClientRect);
    }
  //TODO: Change Brush copy to TImageList
  TRect rect = ::Rect(0,0,FChartBmp->Width,FChartBmp->Height);
  Canvas->BrushCopy(rect,FChartBmp,rect,FMetroData->Color);

//  Canvas->CopyRect(ClientRect,FChartBmp->Canvas,rect);
  if(FLegend)
    {
    TRect r = ::Rect(0,0,FLegendBmp->Width,FLegendBmp->Height);
    if(FMetroData->LegendOrient == otHorz)
      rect = ::Rect(0,rect.Bottom,FLegendBmp->Width,rect.Bottom + FLegendBmp->Height);
    else
      rect = ::Rect(rect.Right,0,rect.Right + FLegendBmp->Width,FLegendBmp->Height);
    Canvas->BrushCopy(rect,FLegendBmp,r,FMetroData->Color);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::SetMetroData(TMetroData * value)
{
  if(FMetroData == value)
    return;
  FMetroData = value;
  FMetroData->OnAfterLoading = LoadComplete;
  if(!FMetroData->Empty)
    LoadComplete(this);
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::LoadComplete(TObject * Sender)
{
  FShow = true;
  if(FMetroData->Background.IsEmpty())
    Color = FMetroData->Color;
  else
    FBkgndBmp->LoadFromFile(FMetroData->Background);
  if(!FMetroData->VisibleName.IsEmpty())
    {
    int w = Canvas->TextWidth(FMetroData->VisibleName),
        h = Canvas->TextHeight(FMetroData->VisibleName);
    Canvas->Brush->Style = bsClear;
    Canvas->TextOut((Width - w) / 2, (Height - h) / 2,FMetroData->VisibleName);
    }
  MapChartData();
  if(FLegend)
    MapLegendData();
  Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::SetCoeff(TFloatPoint value)
{
  if(FCoeff.x == value.x && FCoeff.y == value.y)
    return;
  FCoeff = value;
  FChartBmp->Canvas->Font->Size = Canvas->Font->Size * FCoeff.y;
  MapChartData();
  Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::MapChartData()
{
  HANDLE i;
  HLINE k;
  TColor color;
  //show train passages
  FChartBmp->Width = FMetroData->ChartWidth * FCoeff.x;
  FChartBmp->Height = FMetroData->ChartHeight * FCoeff.y;
  FChartBmp->Canvas->Brush->Color = FMetroData->Color;
  FChartBmp->Canvas->FillRect(::Rect(0,0,FChartBmp->Width,FChartBmp->Height));
  FMetroData->FindInit();
  while ((k = FMetroData->FindLine()) != hNull)
    {
    color = FMetroData->GetColor(k);
    while((i = FMetroData->FindTrainPsg(k)) != hNull)
      PaintTrain(FChartBmp, FMetroData->CoordsOfPassage(i), color, FMetroData->GetPassageDirection(i));
    }
  //show stations and station names
  FMetroData->FindInit();
  TPoint p;
  while ((k = FMetroData->FindLine()) != hNull)
    {
    color = FMetroData->GetColor(k);
    while((i = FMetroData->FindStation(k)) != hNull)
      {
      p = FMetroData->CoordsOfStation(i);
      PaintStation(FChartBmp, p, color);
      PaintStationName(FChartBmp, p, FMetroData->AngleOfStationName(i),FMetroData->CaptionOf(i));
      }
    }
  //show walk passages
  FMetroData->FindInit();
  while ((i = FMetroData->FindWalkPsg()) != hNull)
    PaintWalk(FChartBmp, FMetroData->CoordsOfPassage(i),0);
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::MapLegendData()
{
  TColor color;
  float l1, l2, l3, l;
  int px, py, cols, rows, wd, j1 = 0, j2 = 0, n = 1;
  HLINE k;
  TPoint p1, p2;
  TRect r1, r2;
  if(!FMetroData)
    return;
  if(FMetroData->LegendOrient == loHorz)
    {
    cols = FMetroData->LegendCols;
    l = Width / cols;
    }
  else
    {
    cols = FMetroData->LegendRows;
    l = Height / cols;
    }
  if(FMetroData->LegendNameAlign != naRight && FMetroData->LegendNameAlign != naLeft)
    FMetroData->LegendNameAlign = naRight;
  rows = Ceil((float)(FMetroData->Lines + 1) / cols);
  wd = (FMetroParams->StationParams + 5) * 2 * FCoeff.x;
  l1 = l / 8;
  if(2 * wd > l1)
    l1 = wd * 2;
  l2 = l1 / 2 + (l - l1) * (int) (FMetroData->LegendNameAlign == naLeft);
  if(FMetroData->LegendNameAlign == naLeft)
    n = -1;
  l3 = l1 / 2 * n;
  if(FMetroData->LegendOrient == otHorz)
    {
    FLegendBmp->Width = Width;
    FLegendBmp->Height = rows * wd;
    }
  else
    {
    FLegendBmp->Width = rows * wd;
    FLegendBmp->Height = Height;
    }
//  FLegendBmp->Canvas->Brush->Color = FMetroData->Color;
  FLegendBmp->Canvas->Brush->Color = (TColor) 0xffff00;
  FLegendBmp->Canvas->FillRect(::Rect(0,0,FLegendBmp->Width,FLegendBmp->Height));
  FMetroData->FindInit();

  while ((k = FMetroData->FindLine()) != hNull)
    {
    color = FMetroData->GetColor(k);
    px = l2 + l * j1;
    py = wd / 2 + wd * j2;
    if(FMetroData->LegendOrient == otHorz)
      {
      p1 = Point(px, py);
      p2 = Point(px + l3, py);
      r1 = ::Rect(px - l3, py, px, py);
      r2 = ::Rect(px + l3, py, px, py);
      }
    else
      {
      p1 = Point(py, px);
      p2 = Point(py, px + l3);
      r1 = ::Rect(py, px - l3, py, px);
      r2 = ::Rect(py, px + l3, py, px);
      }
    PaintTrialTrain(FLegendBmp, r1, color);
    PaintTrialTrain(FLegendBmp, r2, color);
    PaintStation(FLegendBmp, p1, color);
    PaintLineName(FLegendBmp, p2, FMetroData->LegendNameAlign, FMetroData->CaptionOf(k));
    if(++j1 == cols)
      {
      j1 = 0;
      if(++j2 == rows)
        j2 = 0;
      }
    }

  px = l2 / 2  + l * j1;
  py = wd / 2 + wd * j2;
  if(FMetroData->LegendOrient == loHorz)
    {
    p1 = Point(px , py);
    p2 = Point(px + wd / 2, py);
    }
  else
    {
    p1 = Point(py, px);
    p2 = Point(py, px + wd / 2);
    }

  HSTATION st1, st2;
  FMetroData->FindInit();
  HPASSAGE i = FMetroData->FindWalkPsg();
  if(i != hNull || FMetroData->GetPassageStations(i,st1,st2))
    {
    PaintStation(FLegendBmp, p1, FMetroData->GetColor(FMetroData->GetLine(st1)));
    PaintStation(FLegendBmp, p2, FMetroData->GetColor(FMetroData->GetLine(st2)));
    PaintWalk(FLegendBmp, ::Rect(p1.x, p1.y, p2.x, p2.y),0);
    }

  if(FMetroData->LegendOrient == loHorz)
    FMetroData->LegendRows = rows;
  else
    FMetroData->LegendCols = rows;
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::PaintStation(Graphics::TBitmap *bmp, TPoint p, TColor color)
{
  int r;
  if(FMetroData->StationBorder)
    bmp->Canvas->Pen->Color = FMetroData->StationBorderColor;
  if (FMetroData->StationBlank)
    {
    r = FMetroParams->StationRadius + FMetroParams->StationBlank;
    if(!FMetroData->StationBorder)
      bmp->Canvas->Pen->Color = FMetroData->StationBlankColor;
    bmp->Canvas->Brush->Color = FMetroData->StationBlankColor;
    bmp->Canvas->Ellipse(Rect(p.x - r, p.y - r, p.x + r + 1, p.y + r + 1,FCoeff));
    bmp->Canvas->Pen->Color = color;
    }
  else
    if(!FMetroData->StationBorder)
      bmp->Canvas->Pen->Color = color;
  r = FMetroParams->StationRadius;
  bmp->Canvas->Brush->Color = color;
  bmp->Canvas->Ellipse(Rect(p.x - r, p.y - r, p.x + r + 1, p.y + r + 1,FCoeff));
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::PaintStationName(Graphics::TBitmap *bmp, TPoint p, unsigned int a, AnsiString name)
{
  if(a == -1)
    return;
  if(a >= 360)
    {
    if(bmp->Height / 2 < p.y)
      a = 180;
    else
      a = 0;
    if(bmp->Width / 2 < p.x)
      a += 90;
    else
      a += 270;
    if(a >= 360)
      a %= 360;
    }
  long double sin, cos;
  SinCos(Pi * a / 180, sin, cos);
  TRect l = GetCalcRect(::Rect(p.x, p.y, p.x, p.y), FMetroParams->StationRadius + FMetroParams->StationBlank + FMetroParams->StationShift, sin, cos);
  bmp->Canvas->Font->Color = FMetroData->MetroObjectNameColor;
  TFontStyles fs;
  fs << fsItalic << fsBold;
  bmp->Canvas->Font->Style = fs;
  if(a == 180 || !a)
    l.Top -= abs(bmp->Canvas->Font->Height) / 2 + 1;
  if(a == 90 || a == 270)
    l.Left -= bmp->Canvas->TextWidth(name) / 2 + 1;
  if(a > 180)
    l.Top -= abs(bmp->Canvas->Font->Height);
  if(a < 90 || a > 270)
    l.Left -= bmp->Canvas->TextWidth(name);
  bmp->Canvas->Brush->Style = bsClear;
  bmp->Canvas->TextOut(l.Left, l.Top, name);
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::PaintTrain(Graphics::TBitmap *bmp, TRect r, TColor color, TPassageDirection dir)
{
  PaintRect(bmp, r, FMetroParams->LineWidth, FMetroParams->StationRadius, FMetroParams->StationBlank + FMetroParams->StationShift,color, rmCutEdges, dir);
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::PaintWalk(Graphics::TBitmap *bmp, TRect r, int angle)
{
  if(angle >= 360)
    return;
  if(!angle || angle == 180)
    return PaintRect(bmp, r, FMetroParams->PassageWidth, FMetroParams->StationRadius + 1, FMetroParams->StationBlank, FMetroData->WalkPsgColor, rmShowBorder, pdWalkTwoDirectional);
  //convert r;

  TRect l = ChordRect(r,angle);
  if(angle > 180)
    Canvas->Arc(l.Left, l.Top, l.Right, l.Bottom, r.Right, r.Bottom, r.Left, r.Top);
  else
    Canvas->Arc(l.Left, l.Top, l.Right, l.Bottom, r.Left, r.Top, r.Right, r.Bottom);
  l = ChordRect(r,angle);
  if(angle > 180)
    Canvas->Arc(l.Left, l.Top, l.Right, l.Bottom, r.Right, r.Bottom, r.Left, r.Top);
  else
    Canvas->Arc(l.Left, l.Top, l.Right, l.Bottom, r.Left, r.Top, r.Right, r.Bottom);
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::PaintRect(Graphics::TBitmap *bmp, TRect r, int len, int shift, int blank, TColor color, TShowRectMode mode, TPassageDirection dir)
{
  int x1, y1;
  long double a, sina, cosa;
  if(r.Left - r.Right)
    a = ArcTan2((r.Top - r.Bottom),(r.Left - r.Right));
  else
    {
    a = Pi / 2;
    if(r.Top - r.Bottom < 0)
      a = -a;
    }
  SinCos(a,sina,cosa);
  TRect l = GetCalcRect(r, shift, sina, cosa);
//  x1 = Round(len * sina / 2);
//  y1 = Round(len * cosa / 2);
  x1 = Floor(len * sina / 2);
  y1 = Floor(len * cosa / 2);
  TPoint p[4];
  p[0] = Point((l.Left + x1) * FCoeff.x, (l.Top - y1) * FCoeff.y);
  p[1] = Point((l.Left - x1) * FCoeff.x, (l.Top + y1) * FCoeff.y);
  p[2] = Point((l.Right - x1) * FCoeff.x, (l.Bottom + y1) * FCoeff.y);
  p[3] = Point((l.Right + x1) * FCoeff.x, (l.Bottom - y1) * FCoeff.y);
  bmp->Canvas->Pen->Color = color;
  bmp->Canvas->Brush->Color = color;
  bmp->Canvas->Polygon(p,3);
  switch(mode)
    {
    case rmCutEdges:
      bmp->Canvas->Pen->Color = FMetroData->Color;
      bmp->Canvas->Brush->Color = FMetroData->Color;
      CutFwdEdge(bmp, r, shift, blank, x1, y1);
      CutBwdEdge(bmp, r, shift, blank, x1, y1);
      break;
    case rmCutFwdEdge:
      bmp->Canvas->Pen->Color = FMetroData->Color;
      bmp->Canvas->Brush->Color = FMetroData->Color;
      CutFwdEdge(bmp, r, shift, blank, x1, y1);
      break;
    case rmCutBwdEdge:
      bmp->Canvas->Pen->Color = FMetroData->Color;
      bmp->Canvas->Brush->Color = FMetroData->Color;
      CutBwdEdge(bmp, r, shift, blank, x1, y1);
      break;
    case rmShowBorder:
      bmp->Canvas->Pen->Color = FMetroData->StationBorderColor;
      bmp->Canvas->MoveTo (p[0].x, p[0].y);
      bmp->Canvas->Pixels[p[3].x][p[3].y] = bmp->Canvas->Pen->Color;
      bmp->Canvas->LineTo (p[3].x, p[3].y);
      bmp->Canvas->MoveTo (p[2].x, p[2].y);
      bmp->Canvas->Pixels[p[1].x][p[1].y] = bmp->Canvas->Pen->Color;
      bmp->Canvas->LineTo (p[1].x, p[1].y);
    }
  switch(dir)
    {
    case pdForwardOnly:
      PaintTriangle(bmp, (r.Left + r.Right) / 2, (r.Top + r.Bottom) / 2, x1, y1, true);
      break;
    case pdBackwardOnly:
      PaintTriangle(bmp, (r.Left + r.Right) / 2, (r.Top + r.Bottom) / 2, x1, y1, false);
      break;
    case pdDifferentWays:
      PaintTriangle(bmp, r.Left + (r.Right - r.Left) / 3, r.Top + (r.bottom - r.Top) / 3, x1, y1, true);
      PaintTriangle(bmp, r.Right - (r.Right - r.Left) / 3, r.Bottom - (r.bottom - r.Top) / 3, x1, y1, false);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::PaintTriangle(Graphics::TBitmap *bmp, int lx, int ly, int x1, int y1, bool fl)
{
  TPoint p[3];
  bmp->Canvas->Pen->Color = FMetroData->Color;
  bmp->Canvas->Brush->Color = FMetroData->Color;
  p[0] = Point((lx - x1) * FCoeff.x, (ly - y1) * FCoeff.y);
  p[1] = Point((lx + x1) * FCoeff.x, (ly + y1) * FCoeff.y);
  if(fl)
    p[2] = Point((lx + y1) * FCoeff.x, (ly + x1) * FCoeff.y);
  else
    p[2] = Point((lx - y1) * FCoeff.x, (ly - x1) * FCoeff.y);
  bmp->Canvas->Polygon(p,2);
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::PaintLineName(Graphics::TBitmap *bmp, TPoint p, TNameAlign na, AnsiString name)
{
  if(na == naLeft)
    p.x -= bmp->Canvas->TextWidth(name);
  p.y -= abs(bmp->Canvas->Font->Height) / 2 + 1;
  bmp->Canvas->Brush->Style = bsClear;
  bmp->Canvas->TextOut(p.x, p.y, name);
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::PaintTrialTrain(Graphics::TBitmap *bmp, TRect r, TColor color)
{
  PaintRect(bmp, r, FMetroParams->LineWidth, FMetroParams->StationRadius, FMetroParams->StationBlank + FMetroParams->StationShift, color, rmCutEdges, pdTwoDirectional);
//  PaintRect(bmp, r, FMetroParams->LineWidth, FMetroParams->StationRadius, FMetroParams->StationBlank + FMetroParams->StationShift, color, rmCutBwdEdge, pdTwoDirectional);
}
//---------------------------------------------------------------------------
TRect __fastcall TMetroChart::GetCalcRect(TRect r, int len, long double sin, long double cos)
{
//  int x1 = Round(len * cos), y1 = Round(len * sin);
  int x1 = Floor(len * cos), y1 = Floor(len * sin);
  return ::Rect(r.Left - x1, r.Top - y1, r.Right + x1, r.Bottom + y1);
}
//---------------------------------------------------------------------------
TRect __fastcall TMetroChart::Rect(int left, int top, int right, int bottom, TFloatPoint coeff)
{
  return ::Rect(left * coeff.x, top * coeff.y, right * coeff.x, bottom * coeff.y);
}
//---------------------------------------------------------------------------
TRect __fastcall TMetroChart::ChordRect(TRect r, int angle)
{
  Extended d, a, b, x, y, sin, cos, beta;
  TRect l;
  int xl = r.Right - r.Left, yl = r.Bottom - r.Top;
  SinCos(((long double) angle) * Pi / 360, sin, cos);//360 is 180 * 2 ( Pi / 360 = (Pi / 180) / 2)
  a = Power(Power(xl, 2) + Power(yl, 2), 0.5) / 2;
  d = 2 * a / sin;
  b = a * cos / 2 / sin;
  if(yl)
    beta = ArcTan2(xl, yl);
  else
    {
    if(xl < 0)
      beta = 3 * Pi / 2;
    else
      beta = Pi / 2;
    }
  SinCos(beta, sin, cos);
  x = (r.Left + r.Right) / 2 - b * sin;
  y = (r.Top + r.Bottom) / 2 + b * cos;
  return ::Rect(Floor(x - d), Floor(y - d), Floor(x + d), Floor(y + d));
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::SetMetroParams(TMetroParams * value)
{
  if(FMetroParams == value)
    return;
  FMetroParams = value;
}
//---------------------------------------------------------------------------
void __fastcall TMetroChart::SetLegend(bool value)
{
  if(FLegend == value)
    return;
  FLegend = value;
  if(value)
    MapLegendData();
}
//---------------------------------------------------------------------------
namespace Metrochart
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMetroChart)};
     RegisterComponents("Metro", classes, 0);
  }
}




void __fastcall TMetroChart::CutFwdEdge(Graphics::TBitmap *bmp, TRect r, int shift, int blank, int x1, int y1)
{
  TRect l = Rect(r.Left - shift - blank, r.Top - shift - blank,
    r.Left + shift + blank, r.Top + shift + blank, FCoeff);
  bmp->Canvas->Chord(l.Left, l.Top, l.Right, l.Bottom, r.Left + x1, r.Top - y1, r.Left - x1, r.Top + y1);
}

void __fastcall TMetroChart::CutBwdEdge(Graphics::TBitmap * bmp, TRect r, int shift, int blank, int x1, int y1)
{
  TRect l = Rect(r.Right - shift - blank, r.Bottom - shift - blank,
    r.Right + shift + blank, r.Bottom + shift + blank, FCoeff);
  bmp->Canvas->Chord(l.Left, l.Top, l.Right, l.Bottom, r.Right - x1, r.Bottom + y1, r.Right + x1, r.Bottom - y1);
}
