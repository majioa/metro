//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MetroObject.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMetroObject *)
{
  new TMetroObject(NULL);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall THistory::THistory()
  : TPersistent()
{
//  FValue = VarArrayCreate();
  FValue = new HISTORYVAL[_DEFAULT_MAX_HISTORY_VALUES_];
  FValueIndex = -1;
}
//---------------------------------------------------------------------------
__fastcall THistory::~THistory()
{
  delete FValue;
}
//---------------------------------------------------------------------------
void __fastcall THistory::SetTime(TDateTime value)
{
  if(!FValues)
    FValueIndex = -1;
  if(FValue[FValueIndex].Time == value)
    return;
  int i;
  for(i = 0; i < FValues; i++)
    if(FValue[i].Time > value)
      {
      FValueIndex = i - 1;
      return;
      }
  FValueIndex = i;
}
//---------------------------------------------------------------------------
bool __fastcall THistory::GetEmpty()
{
  return !FValues;
}
//---------------------------------------------------------------------------
bool __fastcall THistory::GetEmptyValue()
{
  return FValueIndex < 0 && FDefaultValue.IsNull() ||
    FValueIndex >= 0 && FValue[FValueIndex].Value.IsNull();
}
//---------------------------------------------------------------------------
void __fastcall THistory::SetDefaultValue(Variant value)
{
  FDefaultValue = value;
}
//---------------------------------------------------------------------------
void __fastcall THistory::SetDefaultComment(AnsiString value)
{
  FDefaultComment = value;
}
//---------------------------------------------------------------------------
Variant __fastcall THistory::GetValue()
{
  if(FValueIndex < 0)
    return FDefaultValue;
  return FValue[FValueIndex].Value;
}
//---------------------------------------------------------------------------
TDateTime __fastcall THistory::GetTime()
{
  if(FValueIndex < 0)
    return TDateTime(0);
  return FValue[FValueIndex].Time;
}
//---------------------------------------------------------------------------
AnsiString __fastcall THistory::GetComment()
{
  if(FValueIndex < 0)
    return FDefaultComment;
  return FValue[FValueIndex].Comment;
}
//---------------------------------------------------------------------------
void __fastcall THistory::Add(TDateTime Time, Variant Value)
{
  AddComment(Time,Value,"");
}
//---------------------------------------------------------------------------
void __fastcall THistory::AddComment(TDateTime Time, Variant Value, AnsiString Comment)
{
  int i;
  for(i = 0; i < FValues; i++)
    if(FValue[i].Time >= Time)
      {
      if(FValue[i].Time == Time)
        return;
      else
        break;
      }
  Memutils::CheckMemSize((void *)FValue,FMaxValues,FValues,sizeof(HISTORYVAL));
//  Memutils::NewCheckMemSize(FValue,FMaxValues,FValues);
  Memutils::MemCopy(&FValue[i+1],&FValue[i],sizeof(HISTORYVAL)*(FValues - i));
  FValue[i].Time = Time;
  FValue[i].Value = Value;
  FValue[i].Comment = Comment;
  FValues++;
}
//---------------------------------------------------------------------------
void __fastcall THistory::Delete(TDateTime Time)
{
  for(int i = 0; i < FValues; i++)
    if(FValue[i].Time == Time)
      {
      Memutils::MemCopy(&FValue[i],&FValue[i+1],sizeof(HISTORYVAL)*(FValues - i - 1));
      FValues--;
      return;
      }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TMetroObject::TMetroObject(TMetroData* Owner)
  : TPersistent()
{
  FOwner = Owner;
  FOwners = new THistory();
  FNames = new THistory();
  FStates = new THistory();
}
//---------------------------------------------------------------------------
__fastcall TMetroObject::~TMetroObject()
{
  delete FStates;
  delete FNames;
  delete FOwners;
}
//---------------------------------------------------------------------------
void __fastcall TMetroObject::SetDate(TDateTime value)
{
  if(FDate == value)
    return;
  FDate = value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMetroObject::GetName()
{
  return FNames->Value;
}
//---------------------------------------------------------------------------
TMetroState __fastcall TMetroObject::GetState()
{
  return (TMetroState)((int) FStates->Value);
}
//---------------------------------------------------------------------------
HMETROOBJECT __fastcall TMetroObject::GetOwner()
{
  return (HMETROOBJECT)((int) FOwner->Value);
}
//---------------------------------------------------------------------------
/*namespace Metro
{
  void __fastcall PACKAGE Register()
  {
//     TComponentClass classes[1] = {__classid(TMetroObject)};
//     RegisterComponents("Metro", classes, 0);
     TComponentClass classes[1] = {__classid(TMetroStation)};
     RegisterComponents("Metro", classes, 0);
     TComponentClass classes[1] = {__classid(TMetroLine)};
     RegisterComponents("Metro", classes, 0);
     TComponentClass classes[1] = {__classid(TMetroLineSegment)};
     RegisterComponents("Metro", classes, 0);
     TComponentClass classes[1] = {__classid(TMetroPassage)};
     RegisterComponents("Metro", classes, 0);
     TComponentClass classes[1] = {__classid(TMetroData)};
     RegisterComponents("Metro", classes, 0);
  }
}                                            */
//---------------------------------------------------------------------------

static inline void ValidCtrCheck(TMetroStation *)
{
  new TMetroStation(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroStation::TMetroStation(TMetroData *Owner)
  : TMetroObject(Owner)
{
  FType = otStation;
}
//---------------------------------------------------------------------------
void __fastcall TMetroStation::Paint()
{
  //TODO: Add your source code here
}
//---------------------------------------------------------------------------
/*namespace Metrostation
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMetroStation)};
     RegisterComponents("Metro", classes, 0);
  }
} */
//---------------------------------------------------------------------------


static inline void ValidCtrCheck(TMetroLine *)
{
  new TMetroLine(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroLine::TMetroLine(TMetroData *Owner)
  : TMetroObject(Owner)
{
  FType = otLine;
}
//---------------------------------------------------------------------------
/*namespace Metroline
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMetroLine)};
     RegisterComponents("Metro", classes, 0);
  }
} */
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TMetroLineSegment *)
{
  new TMetroLineSegment(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroLineSegment::TMetroLineSegment(TMetroData *Owner)
  : TMetroObject(Owner)
{
  FType = otLineSegment;
}
//---------------------------------------------------------------------------
/*namespace Metrolinesegment
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMetroLineSegment)};
     RegisterComponents("Metro", classes, 0);
  }
} */
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TMetroPassage *)
{
  new TMetroPassage(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroPassage::TMetroPassage(TMetroData *Owner)
  : TMetroObject(Owner)
{
  FType = otPassage;
}
//---------------------------------------------------------------------------
/*namespace Metropassage
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMetroPassage)};
     RegisterComponents("Metro", classes, 0);
  }
} */
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TMetroData *)
{
  new TMetroData(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroData::TMetroData(TComponent *Owner)
  : TComponent(Owner)
{
  FType = otData;
}
//---------------------------------------------------------------------------
/*namespace Metrodata
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMetroData)};
     RegisterComponents("Metro", classes, 0);
  }
} */
//---------------------------------------------------------------------------


void __fastcall TMetroStation::SetStationType(TMetroStationType value)
{
  if(FStationType == value)
    return;
  FStationType = value;
  Paint();
}

void __fastcall TMetroStation::SetUseLineAngle(bool value)
{
  if(FUseLineAngle == value)
    return;
  FUseLineAngle = value;
  Paint();
}

void __fastcall TMetroObject::SetColor(TColor value)
{
  if(FColor == value)
    return;
  FColor = value;
  Paint();
}


void __fastcall TMetroPassage::Paint()
{
  //TODO: Add your source code here
}

void __fastcall TMetroLine::Paint()
{
  //TODO: Add your source code here
}

void __fastcall TMetroLineSegment::Paint()
{
  //TODO: Add your source code here
}

void __fastcall TMetroData::Paint()
{
  //TODO: Add your source code here
}





