//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MetroParams.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMetroParams *)
{
  new TMetroParams(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroParams::TMetroParams(TComponent* Owner)
  : TComponent(Owner)
{
  FStationRadius = _DEFAULT_STATION_RADIUS_;
  FStationBlank = _DEFAULT_STATION_BLANK_;
  FLineWidth = _DEFAULT_LINE_WIDTH_;
  FPassageWidth = _DEFAULT_PASSAGE_WIDTH_;
}
//---------------------------------------------------------------------------
void __fastcall TMetroParams::SetStationRadius(int value)
{
  if(FStationRadius == value)
    return;
    FStationRadius = value;
}
//---------------------------------------------------------------------------
void __fastcall TMetroParams::SetStationBlank(int value)
{
  if(FStationBlank == value)
    return;
  FStationBlank = value;
}
//---------------------------------------------------------------------------
void __fastcall TMetroParams::SetLineWidth(int value)
{
  if(FLineWidth == value)
    return;
  FLineWidth = value;
}
//---------------------------------------------------------------------------
void __fastcall TMetroParams::SetPassageWidth(int value)
{
  if(FPassageWidth == value)
    return;
  FPassageWidth = value;
}
//---------------------------------------------------------------------------
void __fastcall TMetroParams::SetLineOrient(TLineOrient value)
{
  if(FLineOrient == value)
    return;
  FLineOrient = value;
}
//---------------------------------------------------------------------------
void __fastcall TMetroParams::SetLineAngle(TLineAngle value)
{
  if(FLineAngle == value)
    return;
  FLineAngle = value;
}
//---------------------------------------------------------------------------
void __fastcall TMetroParams::SetChartMakingType(TMakingType value)
{
  if(FChartMakingType == value)
    return;
  FChartMakingType = value;
}
//---------------------------------------------------------------------------
void __fastcall TMetroParams::SetStationShift(int value)
{
  if(FStationShift == value)
    return;
  FStationShift = value;
}
//---------------------------------------------------------------------------
namespace Metroparams
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMetroParams)};
     RegisterComponents("Metro", classes, 0);
  }
}
//---------------------------------------------------------------------------
bool __fastcall TMetroParams::LoadIniFile(AnsiString file)
{
  TStringList *list = new TStringList();
  try
    {
    list->LoadFromFile(file);
    int i;
    bool run = false;
    AnsiString name, value;
    for(i = 0; i < list->Count; i++)
      {
      if(!run)
        {
        if(list->Strings[i] == "[�����]")
          run = true;
        continue;
        }
      if(list->Strings[i].IsEmpty())
        continue;
      name = list->Strings[i].GetName();
      if(*((unsigned short *) name.c_str()) == 0x2f2f)
        continue;
      value = list->Strings[i].GetValue();
      if(name == "������_�������")
        {
        FStationRadius = value.ToIntDef(_DEFAULT_STATION_RADIUS_);
        continue;
        }
      if(name == "������_��_�������")
        {
        FStationShift = value.ToIntDef(_DEFAULT_STATION_SHIFT_);
        continue;
        }
      if(name == "����_��������_�������")
        {
        FStationBlank = value.ToIntDef(_DEFAULT_STATION_BLANK_);
        continue;
        }
      if(name == "�������_�����")
        {
        FLineWidth = value.ToIntDef(_DEFAULT_LINE_WIDTH_);
        continue;
        }
      if(name == "�������_��������")
        {
        FPassageWidth = value.ToIntDef(_DEFAULT_PASSAGE_WIDTH_);
        continue;
        }
      if(name == "����������_�����")
        {
        if(value == "�������������")
          {
          FLineOrient = loHorz;
          continue;
          }
        if(value == "������������")
          FLineOrient = loVert;
        continue;
        }
      if(name == "������_�����")
        {
        if(value == "������������")
          {
          FLineAngle = laDiagonal;
          continue;
          }
        if(value == "�������������")
          {
          FLineAngle = laOrthogonal;
          continue;
          }
        if(value == "�����")
          FLineAngle = laAny;
        continue;
        }
      if(name == "���_��������_�����")
        {
        if(value == "����������")
          {
          FChartMakingType = mtStandard;
          continue;
          }
        if(value == "������")
          {
          FChartMakingType = mtOld;
          continue;
          }
        if(value == "���������")
          FChartMakingType = mtOldest;
        }
      }
    delete list;
    }
  catch(...)
    {
    delete list;
    return false;
    }
  return true;
}
//---------------------------------------------------------------------------
int __fastcall TMetroParams::GetStationParams()
{
  return FStationRadius + FStationBlank + FStationShift;
}
