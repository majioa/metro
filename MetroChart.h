//---------------------------------------------------------------------------
#ifndef MetroObjectH
#define MetroObjectH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Memutils.h>
//---------------------------------------------------------------------------
#define _DEFAULT_MAX_HISTORY_VALUES_        32
//---------------------------------------------------------------------------
DECLARE_HANDLE(HMETROOBJECT);
//---------------------------------------------------------------------------
#pragma option push -b
enum TMetroState {msImage, msInProject, msBuild, msWork, msRepair, msExpires};
enum TMetroObjectType {otData, otLine, otLineSegment, otStation, otPassage};
enum TMetroStationType {stBitmap, stCircle, stOldType, stRectngle, stSquare};
enum TMetroLineType {ltThin, ltStdLine, ltRoundedLine};
#pragma option pop
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#pragma pack (push,1)
typedef struct {
  Variant Value;
  TDateTime Time;
  AnsiString Comment;
} HISTORYVAL, *PHISTORYVAL;
#pragma pack (pop)
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#pragma pack (push,4)
class PACKAGE THistory : public TPersistent
{
private:
//  Variant FValue;
  PHISTORYVAL FValue;
  int FValues;
  int FMaxValues;
  int FValueIndex;
  Variant FDefaultValue;
  AnsiString FDefaultComment;
  void __fastcall SetTime(TDateTime value);
  bool __fastcall GetEmpty();
  bool __fastcall GetEmptyValue();
  Variant __fastcall GetValue();
  TDateTime __fastcall GetTime();
  AnsiString __fastcall GetComment();
protected:
public:
  __fastcall THistory();
  void __fastcall SetDefaultValue(Variant value);
  void __fastcall SetDefaultComment(AnsiString value);
  __fastcall ~THistory();
  void __fastcall Add(TDateTime Time, Variant Value);
  void __fastcall AddComment(TDateTime Time, Variant Value, AnsiString Comment);
  void __fastcall Delete(TDateTime Time);
  int __fastcall IndexOf(TDateTime Time);
  __property Variant Value  = { read=GetValue };
  __property TDateTime Time  = { read=GetTime, write=SetTime };
  __property AnsiString Comment  = { read=GetComment };
  __property Variant DefaultValue  = { read=FDefaultValue, write=FDefaultValue };
  __property AnsiString DefaultComment  = { read=FDefaultComment, write=FDefaultComment };
  __property bool Empty  = { read=GetEmpty };
  __property bool EmptyValue  = { read=GetEmptyValue };
  __property int Values  = { read=FValues };
__published:
};
//---------------------------------------------------------------------------
class PACKAGE TMetroData : public TComponent
{
private:
protected:
public:
  __fastcall TMetroData(TComponent *Owner);
  void __fastcall Paint();
__published:
};
//---------------------------------------------------------------------------
class PACKAGE TMetroObject : public TPersistent
{
private:
  TMetroData *FOwner;
  THistory *FNames;
  THistory *FStates;
  THistory *FOwners;
  HMETROOBJECT FHandle;
  AnsiString FIntName;
  AnsiString FComment;
  TCanvas *FCanvas;
  TColor FColor;
  TDateTime FDate;
  void __fastcall SetColor(TColor value);
  virtual void __fastcall SetDate(TDateTime value);
  AnsiString __fastcall GetName();
  TMetroState __fastcall GetState();
  HMETROOBJECT __fastcall GetOwner();
protected:
  TMetroObjectType FType;
  virtual void __fastcall Paint() = 0;
public:
  __fastcall TMetroObject(TMetroData* Owner);
  __fastcall ~TMetroObject();
//  __property TMetroData *Owner = {read=FOwner};
  __property THistory *Names  = { read=FNames };
  __property THistory *States  = { read=FStates };
  __property THistory *Owners  = { read=FOwners };
  __property HMETROOBJECT Handle  = { read=FHandle, write=FHandle };
  __property AnsiString IntName  = { read=FIntName, write=FIntName };
  __property AnsiString Comment  = { read=FComment, write=FComment };
  __property TCanvas *Canvas  = { read=FCanvas, write=FCanvas };
  __property TMetroObjectType Type  = { read=FType };
  __property TColor Color  = { read=FColor, write=SetColor };
  __property TDateTime Date  = { read=FDate, write=SetDate };
  __property AnsiString Name  = { read=GetName };
  __property TMetroState State  = { read=GetState };
  __property HMETROOBJECT Owner  = { read=GetOwner };
__published:
};
//---------------------------------------------------------------------------
class PACKAGE TMetroStation : public TMetroObject
{
private:
  TMetroStationType FStationType;
  bool FUseLineAngle;
  void __fastcall SetStationType(TMetroStationType value);
  void __fastcall SetUseLineAngle(bool value);
protected:
public:
  __fastcall TMetroStation(TMetroData *Owner);
  void __fastcall Paint();
  __property TMetroStationType StationType  = { read=FStationType, write=SetStationType };
  __property bool UseLineAngle  = { read=FUseLineAngle, write=SetUseLineAngle };
__published:
};
//---------------------------------------------------------------------------
class PACKAGE TMetroLine : public TMetroObject
{
private:
protected:
public:
  __fastcall TMetroLine(TMetroData *Owner);
  void __fastcall Paint();
__published:
};
//---------------------------------------------------------------------------
class PACKAGE TMetroLineSegment : public TMetroObject
{
private:
protected:
public:
  __fastcall TMetroLineSegment(TMetroData *Owner);
  void __fastcall Paint();
__published:
};
//---------------------------------------------------------------------------
class PACKAGE TMetroPassage : public TMetroObject
{
private:
protected:
public:
  __fastcall TMetroPassage(TMetroData *Owner);
  void __fastcall Paint();
__published:
};
//---------------------------------------------------------------------------
class PACKAGE TMetroChart : public TCustomControl
{
private:
protected:
public:
  __fastcall TMetroChart(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#pragma pack (pop)
//---------------------------------------------------------------------------
#endif
