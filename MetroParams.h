//---------------------------------------------------------------------------

#ifndef MetroParamsH
#define MetroParamsH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#define _DEFAULT_STATION_RADIUS_             5
#define _DEFAULT_STATION_BLANK_              2
#define _DEFAULT_STATION_SHIFT_              4
#define _DEFAULT_LINE_WIDTH_                 8
#define _DEFAULT_PASSAGE_WIDTH_              4

enum TLineOrient {loHorz, loVert};
enum TLineAngle {laDiagonal, laAny, laOrthogonal};
enum TMakingType {mtStandard, mtOld, mtOldest};//тип рисования схемы
enum TShowRectMode {rmNormal, rmCutEdges, rmCutFwdEdge, rmCutBwdEdge, rmShowBorder};

class PACKAGE TMetroParams : public TComponent
{
private:
  int FStationRadius;
  int FStationBlank;
  int FStationShift;
  int FLineWidth;
  int FPassageWidth;
  TLineOrient FLineOrient;
  TLineAngle FLineAngle;
  TMakingType FChartMakingType;
  void __fastcall SetStationRadius(int value);
  void __fastcall SetStationBlank(int value);
  void __fastcall SetStationShift(int value);
  void __fastcall SetLineWidth(int value);
  void __fastcall SetPassageWidth(int value);
  void __fastcall SetLineOrient(TLineOrient value);
  void __fastcall SetLineAngle(TLineAngle value);
  void __fastcall SetChartMakingType(TMakingType value);
  int __fastcall GetStationParams();
protected:
public:
  __fastcall TMetroParams(TComponent* Owner);
  bool __fastcall LoadIniFile(AnsiString);
  __property int StationParams  = { read=GetStationParams };
__published:
  __property int StationRadius  = { read=FStationRadius, write=SetStationRadius };
  __property int StationBlank  = { read=FStationBlank, write=SetStationBlank };
  __property int StationShift  = { read=FStationShift, write=SetStationShift };
  __property int LineWidth  = { read=FLineWidth, write=SetLineWidth };
  __property int PassageWidth  = { read=FPassageWidth, write=SetPassageWidth };
  __property TLineOrient LineOrient  = { read=FLineOrient, write=SetLineOrient };
  __property TLineAngle LineAngle  = { read=FLineAngle, write=SetLineAngle };
  __property TMakingType ChartMakingType  = { read=FChartMakingType, write=SetChartMakingType };
};
//---------------------------------------------------------------------------
#endif
