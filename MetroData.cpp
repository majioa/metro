//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MetroData.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
__fastcall TMetroObject::TMetroObject(TMetroData* Owner, HANDLE hndl)
  : TPersistent()
{
  if(!Owner)
    {
    delete this;
    throw EWrongOwner("Metro data have not been setted");
    }
  FOwner = Owner;
  FHandle = hndl;
//  FNull = TDate ("1.1.1800");
  FMaxInProjects = _DEFAULT_MAX_IN_PROJECT_TIMES_;
  FInProject = new TDate[_DEFAULT_MAX_IN_PROJECT_TIMES_];
  FMaxWorks = _DEFAULT_MAX_WORK_TIMES_;
  FWork = new TDate[_DEFAULT_MAX_WORK_TIMES_];
}
//---------------------------------------------------------------------------
__fastcall TMetroObject::~TMetroObject()
{
  if (FCaptions)
    delete FCaptions;
  delete FWork;
  delete FInProject;
}
//---------------------------------------------------------------------------
void __fastcall TMetroObject::Clear()
{
  FLastError = _ER_OK_;
  int i;
  FInProjects = 0;
  FWorks = 0;
  FName = "";
  FCaption = "";
}
//---------------------------------------------------------------------------
void __fastcall TMetroObject::AddProjectDate(AnsiString str)
{
  AddTimes(str,FInProject,FInProjects,FMaxInProjects,true);
}
//---------------------------------------------------------------------------
void __fastcall TMetroObject::AddWorkDate(AnsiString str)
{
  AddTimes(str,FWork,FWorks,FMaxWorks,false);
}
//---------------------------------------------------------------------------
void __fastcall TMetroObject::AddTimes(AnsiString str, TDate * date, unsigned char &dates, unsigned char &maxdates, bool fl)
{
  int k = str.Pos(","), i;
  FLastError = _ER_OK_;
  TDate dat1, dat2;
  if(k)
    {
    dat1 = TDate(str.SubString(1, k - 1));
    dat2 = TDate(str.SubString(k + 1, str.Length() - k));
    }
  else
    {
    dat1 = TDate(str);
//    dat2 = FNull;
    dat2 = 0;
    }
  for(i = 0; i < dates; i++)
    if(date[i << 1] == dat1 && date[i << 1 + 1] == dat2)
      break;
  if(i != dates)
    {
    if(fl)
      FLastError = _ER_IN_PROJECT_TIMES_ALREADY_EXIST_;
    else
      FLastError = _ER_WORK_TIMES_ALREADY_EXIST_;
    return;
    }
  if(i == maxdates)
    {
    maxdates *= 2;
    TDate *p = new TDate[maxdates];
    memcpy (p,date,sizeof(TDate) * i);
    delete date;
    date = p;
    }
  date[dates << 1] = dat1;
  date[dates++ << 1 + 1] = dat2;
}
//---------------------------------------------------------------------------
bool __fastcall TMetroObject::IsInProject(TDate date)
{
  FLastError = _ER_OK_;
  int i;
  for (i = 0; i < FInProjects; i++)
    if(FInProject[i << 2] >= date && FInProject[i << 2 + 1] < date)
      break;
  return i != FInProjects;
}
//---------------------------------------------------------------------------
bool __fastcall TMetroObject::IsWorking(TDate date)
{
  FLastError = _ER_OK_;
  int i;
  for (i = 0; i < FWorks; i++)
    if(FWork[i << 2] >= date && FWork[i << 2 + 1] < date)
      break;
  return i != FWorks;
}
//---------------------------------------------------------------------------
bool __fastcall TMetroObject::IsExpired(TDate date)
{
  return !(IsWorking(date) || IsInProject(date));
}
//---------------------------------------------------------------------------
bool __fastcall TMetroObject::IsNameTrue(TDate date)
{
  int i, k, l;
  FLastError = _ER_OK_;
  if (!FCaptions)
    return true;
  for (i = 0; i < FCaptions->Count; i++)
    {
    k = FCaptions->Strings[i].Pos(",");
    if (!k && FCaptions->Strings[i] == Caption ||
         k && FCaptions->Strings[i].SubString(1, k - 1) == FCaption)
      break;
    }
  if (FCaptions->Count == i)
    {
    FLastError = _ER_OUT_OF_NAMES_;
    return true;
    }
  if (!k)
    return true;
  AnsiString date1 = FCaptions->Strings[i].SubString(k + 1, FCaptions->Strings[i].Length() - k), date2;
  l = date1.Pos(",");
  if (l)
    {
    date1 = FCaptions->Strings[i].SubString(1, l - 1);
    date2 = FCaptions->Strings[i].SubString(l + 1, FCaptions->Strings[i].Length() - l);
    }
  return StrToDate(date1) <= date && (date2.IsEmpty() || StrToDate(date2) > date);
}
//---------------------------------------------------------------------------
bool __fastcall TMetroObject::SetCaptionByDate(TDate date)
{
  FLastError = _ER_OK_;
  if (!FCaptions)
    return true;
  int i, k, l;
  TDateTime date1 = 0, date2 = 0;
  bool fl;
  for (i = 0; i < FCaptions->Count; i++)
    {
    fl = false;
//    k = FCaptions->Strings[i].FindFirst(",");
//    l = FCaptions->Strings[i].FindNext(",");
    k = FindSubStr(FCaptions->Strings[i],",");
    l = FindSubStr(FCaptions->Strings[i],",");
    if (k)
      {
      date1 = StrToDate(FCaptions->Strings[i].SubString(k + 1, l - k - 1));
      fl = date1 < date;
      if (l)
        {
        date2 = StrToDate(FCaptions->Strings[i].SubString(l + 1, FCaptions->Strings[i].Length()));
        fl = fl && date2 < date;
        }
      else
        date2 = 0;
      }
    if (fl)
      break;
    }
  if (i == FCaptions->Count)
    {
    FLastError = _ER_OUT_OF_NAMES_;
    return false;
    }
  if (k)
    FCaption = FCaptions->Strings[i].SubString(0, k - 1);
  else
    FCaption = FCaptions->Strings[i];
  FNameSetted = date1;
  FNameExpired = date2;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TMetroObject::AddCaption(AnsiString caption)
{
  int i, k, l;
  if (FCaption.IsEmpty())
    {
    k = FindSubStr(caption,",");
    l = FindSubStr(caption,",");
    if (k)
      {
      FCaption = caption.SubString(1, k - 1);
      if (l)
        {
        FNameSetted = StrToDate(caption.SubString(k + 1, l - k - 1));
        FNameExpired = StrToDate(caption.SubString(l + 1, caption.Length() - l));
        }
      else
        {
        FNameSetted = StrToDate(caption.SubString(k + 1, caption.Length() - k));
        FNameExpired = 0;
        }
      }
    else
      {
      FCaption = caption;
      FNameSetted = 0;
      FNameExpired = 0;
      }
    return;
    }
  if (!FCaptions)
    {
    FCaptions = new TStringList();
    FCaptions->Add(FCaption + "," + FNameSetted.DateString() + "," + FNameExpired.DateString());
    }
  FCaptions->Add(caption);
}
//---------------------------------------------------------------------------
int __fastcall TMetroObject::FindSubStr(AnsiString str, AnsiString substr)
{
  if(FSubStr != substr)
    {
    FSubStr = substr;
    FFindIndex = 0;
    }
  int l1 = str.Length() + 1, l2 = substr.Length() + 1, i ,j;
  for(i = FFindIndex + 1; i <= l1 - l2 + 1; i++)
    {
    for(j = 1; j < l2; j++)
      if(str[i] != substr[j])
        break;
    if(j == l2)
      {
      FFindIndex = i;
      return i;
      }
    else
      i += j - 1;
    }
  FFindIndex = 0;
  return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TMetroStation::TMetroStation(TMetroData* Owner, HSTATION id)
  : TMetroObject(Owner, id)
{
  FWalkPassage = new HPASSAGE[_DEFAULT_WALK_PASSAGES_PER_STATION_];
  FMaxWalkPassages = _DEFAULT_WALK_PASSAGES_PER_STATION_;
  FTrainPassage = new HPASSAGE[_DEFAULT_TRAIN_PASSAGES_PER_STATION_];
  FMaxTrainPassages = _DEFAULT_TRAIN_PASSAGES_PER_STATION_;
  FLine = hNull;
}
//---------------------------------------------------------------------------
__fastcall TMetroStation::~TMetroStation()
{
  delete TrainPassage;
  delete WalkPassage;
}
//---------------------------------------------------------------------------
void __fastcall TMetroStation::Clear()
{
  TMetroObject::Clear();
  FWalkPassages = 0;
  FTrainPassages = 0;
  FLine = hNull;
}
//---------------------------------------------------------------------------
bool __fastcall TMetroStation::AssignPassage(HPASSAGE psg, TPassageType type)
{
  if(type == ptWalk)
    return AssignPassage(psg, FWalkPassage, FWalkPassages, FMaxWalkPassages);
  return AssignPassage(psg, FTrainPassage, FTrainPassages, FMaxTrainPassages);
}
//---------------------------------------------------------------------------
bool __fastcall TMetroStation::AssignPassage(HPASSAGE psg, HPASSAGE * &p, unsigned int &passages, unsigned int &maxpassages)
{
  unsigned int i;
  for (i = 0; i < passages; i++)
    if(p[i] == psg)
      {
      FLastError = _ER_PASSAGE_ALREADY_EXIST_;
      return false;
      }
  if (i == maxpassages)
    {
    maxpassages *= 2;
    HSTATION *p1 = new HSTATION[maxpassages];
    memcpy (p1,p,i);
    delete p;
    p = p1;
    }
  p[i] = psg;
  passages++;
  FLastError = _ER_OK_;
  return true;
}
//---------------------------------------------------------------------------
HPASSAGE __fastcall TMetroStation::GetFirstWalk()
{
  FWalkIndex = 0;
  FLastError = _ER_OK_;
  if (FWalkIndex != FWalkPassages)
    return FWalkPassage[FWalkIndex];
  FLastError = _ER_NO_MORE_WALK_PASSAGES_;
  return _WRONG_HANDLE_;
}
//---------------------------------------------------------------------------
HPASSAGE __fastcall TMetroStation::GetNextWalk()
{
  FWalkIndex++;
  FLastError = _ER_OK_;
  if (FWalkIndex != FWalkPassages)
    return FWalkPassage[FWalkIndex];
  FLastError = _ER_NO_MORE_WALK_PASSAGES_;
  return _WRONG_HANDLE_;
}
//---------------------------------------------------------------------------
HPASSAGE __fastcall TMetroStation::GetFirstTrain()
{
  FTrainIndex = 0;
  FLastError = _ER_OK_;
  if (FTrainIndex != FTrainPassages)
    return FTrainPassage[FTrainIndex];
  FLastError = _ER_NO_MORE_TRAIN_PASSAGES_;
  return _WRONG_HANDLE_;
}
//---------------------------------------------------------------------------
HPASSAGE __fastcall TMetroStation::GetNextTrain()
{
  FTrainIndex++;
  FLastError = _ER_OK_;
  if (FTrainIndex != FTrainPassages)
    return FTrainPassage[FTrainIndex];
  FLastError = _ER_NO_MORE_TRAIN_PASSAGES_;
  return _WRONG_HANDLE_;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TMetroPassage::TMetroPassage(TMetroData* Owner, HPASSAGE id, TPassageType type)
  : TMetroObject(Owner, id)
{
  FType = type;
  Clear();
}
//---------------------------------------------------------------------------
__fastcall TMetroPassage::~TMetroPassage()
{
  //TODO: Add your source code here
}
//---------------------------------------------------------------------------
void __fastcall TMetroPassage::Clear()
{
  TMetroObject::Clear();
  FFirstStation = hNull;
  FSecondStation = hNull;
  FLine = hNull;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TMetroLine::TMetroLine(TMetroData* Owner, HLINE id)
  : TMetroObject(Owner, id)
{
  FMaxStations = _DEFAULT_MAX_STATIONS_PER_LINE_;
  FStation = new HSTATION[_DEFAULT_MAX_STATIONS_PER_LINE_];
}
//---------------------------------------------------------------------------
__fastcall TMetroLine::~TMetroLine()
{
  delete FStation;
}
//---------------------------------------------------------------------------
void __fastcall TMetroLine::Clear()
{
  TMetroObject::Clear();
  FColor = clBlack;
  FStations = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMetroLine::AddStation(HSTATION stat)
{
  unsigned int i;
  for(i = 0; i < FStations; i++)
    if(FStation[i] == stat)
      break;
  if(i != FStations)
    {
    FLastError = _ER_STATION_ALREADY_EXIST_;
    return;
    }
  if(i == FMaxStations)
    {
    FMaxStations *= 2;
    HSTATION *p = new HSTATION[FMaxStations];
    memcpy(p,FStation,sizeof(HSTATION) * i);
    delete FStation;
    FStation = p;
    }
  FStation[i] = stat;
  FStations++;
}
//---------------------------------------------------------------------------
bool __fastcall TMetroLine::RemoveStation(HSTATION stat)
{
  unsigned int i;
  FLastError = _ER_OK_;
  for(i = 0; i < FStations; i++)
    if(FStation[i] == stat)
      break;
  if(i == FStations)
    {
    FLastError = _ER_STATION_NOT_FOUND_;
    return false;
    }
  for(++i; i < FStations; i++)
    FStation[i - 1] = FStation[i];
  FStations--;
  return true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TMetroData *)
{
  new TMetroData(NULL);
}
//---------------------------------------------------------------------------
__fastcall TMetroData::TMetroData(TComponent* Owner)
  : TComponent(Owner)
{
  FStation = new TMetroStation *[_DEFAULT_MAX_STATIONS_];
  FMaxStations = _DEFAULT_MAX_STATIONS_;
  FWalkPassage = new TMetroPassage *[_DEFAULT_MAX_WALK_PASSAGES_];
  FMaxWalkPassages = _DEFAULT_MAX_WALK_PASSAGES_;
  FTrainPassage = new TMetroPassage *[_DEFAULT_MAX_TRAIN_PASSAGES_];
  FMaxTrainPassages = _DEFAULT_MAX_TRAIN_PASSAGES_;
  FLine = new TMetroLine *[_DEFAULT_MAX_LINES_];
  FMaxLines = _DEFAULT_MAX_LINES_;
  FEmpty = true;
  FDate = TDate::CurrentDate();
  FLegendNameAlign = naRight;
}
//---------------------------------------------------------------------------
__fastcall TMetroData::~TMetroData()
{
  Clear();
  delete FStation;
  delete FWalkPassage;
  delete FTrainPassage;
  delete FLine;
}
//---------------------------------------------------------------------------
void __fastcall TMetroData::Clear()
{
  unsigned int i;
  for(i = 0; i < FStations; i++)
    delete FStation[i];
  FStations = 0;
  for(i = 0; i < FLines; i++)
    delete FLine[i];
  FLines = 0;
  for(i = 0; i < FWalkPassages; i++)
    delete FWalkPassage[i];
  FWalkPassages = 0;
  for(i = 0; i < FTrainPassages; i++)
    delete FTrainPassage[i];
  FTrainPassages = 0;
  FEmpty = true;
}
//---------------------------------------------------------------------------
void __fastcall TMetroData::SetMetroFile(AnsiString value)
{
  if(FMetroFile == value)
    return;
  FMetroFile = value;
}
//---------------------------------------------------------------------------
void __fastcall TMetroData::SetColor(TColor color)
{
  if(FColor == color)
    return;
  FColor = color;
}
//---------------------------------------------------------------------------
void __fastcall TMetroData::SetBackground(AnsiString value)
{
  if(FBackground == value)
    return;
  FBackground = value;
}
//---------------------------------------------------------------------------
bool __fastcall TMetroData::LoadMetroFile(AnsiString file)
{
  if(!FileExists(file))
    {
    FLastError = _ER_METRO_FILE_NOT_FOUND_;
    return false;
    }
  TFileStream *f;
  try
    {
    f = new TFileStream(file,fmOpenRead);
    }
  catch(...)
    {
    FLastError = _ER_METRO_FILE_CANNOT_BE_OPEN_ ;
    return false;
    }
  char buf[8];
  int l = f->Read(buf,7);
  f->Position = 0;
  bool fl;
  if(l < 7)
    {
    FLastError = _ER_ERROR_IN_FILE;
    return false;
    }
  buf[7] = '\0';
  if(AnsiString(buf) == "[Ã≈“–Œ]")
    {
    fl = LoadText(f);
    }
  else
    {
    if(*((unsigned int *) buf) == 0x1441ebbe)
      fl = LoadBinary(f);
    else
      {
      delete f;
      FLastError = _ER_UNKNOWN_FILE_FORMAT_;
      return false;
      }
    }
  delete f;
  FEmpty = false;
  if(FOnAfterLoading)
    FOnAfterLoading(this);
  if(FOnLoadComplete)
    FOnLoadComplete(this);
  return fl;
}
//---------------------------------------------------------------------------
bool __fastcall TMetroData::LoadText(TFileStream * file)
{
  Clear();
  TStringList *list = new TStringList();
  list->LoadFromStream(file);
//  bool run = false;
  unsigned int i, j, k, l;
  TMetroEnterMode mode = memIgnore;
  AnsiString name,value;
  for (i = 0; i < (unsigned int) list->Count; i++)
    {
    if(FOnLoadProgress)
      FOnLoadProgress(this,i * 100 / list->Count);
    if(list->Strings[i].IsEmpty())
      continue;
    name = list->Strings[i].GetName();
    if(*((unsigned short *) name.c_str()) == 0x2f2f)
      continue;
    value = list->Strings[i].GetValue();
    if(name == "[—’≈Ã¿]")
      {
      mode = memChart;
      continue;
      }
    if(name == "[À»Õ»ﬂ]")
      {
      mode = memLine;
      for (k = 0; k < FLines; k++)
        if(FLine[k]->Name == value)
          {
          FSelLine = FLine[k];
          break;
          }
      if(k == FLines)
        memIgnore;
      continue;
      }
    if(name == "[—“¿Õ÷»ﬂ]")
      {
      mode = memStation;
      for (k = 0; k < FStations; k++)
        if(FStation[k]->Name == value)
          {
          FSelStation = k;
          break;
          }
      if(k == FStations)
        memIgnore;
      continue;
      }
    if(name == "[œ≈–≈’Œƒ]")
      {
      mode = memWalkPsg;
      for (k = 0; k < FWalkPassages; k++)
        if(FWalkPassage[k]->Name == value)
          {
          FSelWalk = FWalkPassage[k];
          break;
          }
      if(k == FWalkPassages)
        memIgnore;
      continue;
      }
    if(name == "[œ–Œ≈«ƒ]")
      {
      mode = memTrainPsg;
      for (k = 0; k < FTrainPassages; k++)
        if(FTrainPassage[k]->Name == value)
          {
          FSelTrain = FTrainPassage[k];
          break;
          }
      if(k == FTrainPassages)
        memIgnore;
      continue;
      }
    if(name == "Õ¿«¬¿Õ»≈")
      {
      switch(mode)
        {
        case memChart:
          FVisibleName = value;
          break;
        case memLine:
          FSelLine->AddCaption(value);
          break;
        case memStation:
          FStation[FSelStation]->AddCaption(value);
          break;
        case memWalkPsg:
          FSelWalk->AddCaption(value);
          break;
        case memTrainPsg:
          FSelTrain->AddCaption(value);
        }
      continue;
      }
    if(name == "«¿√ŒÀŒ¬Œ ")
      {
      if(mode == memChart)
        Caption = value;
      continue;
      }
    if(name == "‘ŒÕ")
      {
      if(FileExists(value))
        FBackground = value;
      continue;
      }
    if(name == "÷¬≈“")
      {
      switch(mode)
        {
        case memChart:
          FColor = (TColor) value.ToIntDef(-1);
          break;
        case memLine:
          FSelLine->Color = (TColor) value.ToIntDef(0);
        }
      continue;
      }
    if(name == "÷¬≈“_ –¿ﬂ_—“¿Õ÷»»")
      {
      if(mode == memChart)
        {
        FStationBorderColor = (TColor) value.ToIntDef(-1);
        FStationBorder = true;
        }
      continue;
      }
    if(name == "÷¬≈“_œ≈–≈’Œƒ¿")
      {
      if(mode == memChart)
        FWalkPsgColor = (TColor) value.ToIntDef(-1);
      continue;
      }
    if(name == "÷¬≈“_Õ¿«¬¿Õ»ﬂ")
      {
      if(mode == memChart)
        FMetroObjectNameColor = (TColor) value.ToIntDef(-1);
      continue;
      }
    if(name == "÷¬≈“_—“»–¿Õ»ﬂ_—“¿Õ÷»»")
      {
      if(mode == memChart)
        {
        FStationBlankColor = (TColor) value.ToIntDef(-1);
        FStationBlank = true;
        }
      continue;
      }
    if(name == "› —œÀ”¿“¿÷»ﬂ")
      {
      switch(mode)
        {
        case memLine:
          FSelLine->AddWorkDate(value);
          break;
        case memStation:
          FStation[FSelStation]->AddWorkDate(value);
          break;
        case memWalkPsg:
          FSelWalk->AddWorkDate(value);
          break;
        case memTrainPsg:
          FSelTrain->AddWorkDate(value);
        }
      continue;
      }
    if(name == "À»Õ»ﬂ")
      {
      if(mode == memChart)
        {
        FLine[FLines] = new TMetroLine(this,(HLINE) FLines);
        FLine[FLines++]->Name = value;
        }
      continue;
      }
    if(name == "—“¿Õ÷»ﬂ")
      {
      if(mode == memLine)
        {
        FStation[FStations] = new TMetroStation(this,(HSTATION) MAKELONG(FStations,1));
        FStation[FStations]->Name = value;
        FStation[FStations]->Line = FSelLine->Handle;
        FSelLine->AddStation(FStation[FStations++]->Handle);
        }
      continue;
      }
/*    if(name == "œ≈–≈’Œƒ")
    if(name == "œ–Œ≈«ƒ")*/
    if(name == "√Œ–»« ŒŒ–ƒ")
      {
      if(mode == memStation)
        FStation[FSelStation]->X = value.ToIntDef(-1);
      continue;
      }
    if(name == "¬≈–“ ŒŒ–ƒ")
      {
      if(mode == memStation)
        FStation[FSelStation]->Y = value.ToIntDef(-1);
      continue;
      }
    if(name == "ÿ»–»Õ¿")
      {
      if(mode == memChart)
        FChartWidth = value.ToIntDef(-1);
      continue;
      }
    if(name == "¬€—Œ“¿")
      {
      if(mode == memChart)
        FChartHeight = value.ToIntDef(-1);
      continue;
      }
    if(name == "œ–Œ≈«ƒ")
      {
      if(mode == memStation)
        CalcPassage(value,FTrainPassages,FMaxTrainPassages,FTrainPassage,ptTrain);
      continue;
      }
    if(name == "œ≈–≈’Œƒ")
      {
      if(mode == memStation)
        CalcPassage(value,FWalkPassages,FMaxWalkPassages,FWalkPassage,ptWalk);
      continue;
      }
    if(name == "œ–Œ≈ “")
      {
      switch(mode)
        {
        case memLine:
          FSelLine->AddProjectDate(value);
          break;
        case memStation:
          FStation[FSelStation]->AddProjectDate(value);
          break;
        case memWalkPsg:
          FSelWalk->AddProjectDate(value);
          break;
        case memTrainPsg:
          FSelTrain->AddProjectDate(value);
        }
      continue;
      }
    if(name == "¿¬“Œ–")
      {
      if(mode == memChart)
        FChartAuthor = value;
      continue;
      }
    if(name == "¬€–¿¬Õ»¬¿Õ»≈")
      {
      TNameAlign val = naAuto;
      do
        {
        if(value == "—À≈¬¿")
          {
          val = naLeft;
          break;
          }
        if(value == "—œ–¿¬¿")
          {
          val = naRight;
          break;
          }
        if(value == "—Õ»«”")
          {
          val = naBottom;
          break;
          }
        if(value == "—¬≈–’”")
          {
          val = naTop;
          break;
          }
        if(value == "—¬≈–’”—À≈¬¿")
          {
          val = naTopLeft;
          break;
          }
        if(value == "—¬≈–’”—œ–¿¬¿")
          {
          val = naTopRight;
          break;
          }
        if(value == "—Õ»«”—À≈¬¿")
          {
          val = naBottomLeft;
          break;
          }
        if(value == "—Õ»«”—œ–¿¬¿")
          val = naBottomRight;
        } while (false);
      switch(mode)
        {
        case memStation:
          FStation[FSelStation]->NameAlign = val;
          break;
        case memTrainPsg:
          FSelTrain->NameAlign = val;
          break;
        case memWalkPsg:
          FSelWalk->NameAlign = val;
        }
      continue;
      }
    }
  RefreshDate();
  if(FOnLoadProgress)
    FOnLoadProgress(this,100);
  delete list;
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TMetroData::LoadBinary(TFileStream * file)
{
  Clear();
  //TODO: Add your source code here
  return false;
}
//---------------------------------------------------------------------------
/*bool __fastcall TMetroData::SaveText(TFileStream * file)
{
  //TODO: Add your source code here
}
//---------------------------------------------------------------------------
bool __fastcall TMetroData::SaveBinary(TFileStream * file)
{
  //TODO: Add your source code here
} */
//---------------------------------------------------------------------------
void __fastcall TMetroData::CalcPassage(AnsiString value, unsigned int & passages, unsigned int & maxpassages, TMetroPassage ** passage, TPassageType mode)
{
  unsigned int k, l, j = value.Pos(",");
  AnsiString name;
  if(j)
    name = value.SubString(1, j - 1);
  else
    name = value;
  for (k = 0; k < FStations; k++)
    if(FStation[k]->Name == name)
      break;
  if(k == FStations)
    {
    FLastError = _ER_STATION_ALREADY_EXIST_;
    return;
    }
  for(l = 0; l < passages; l++)
    if(passage[l]->SecondStation == FStation[FSelStation]->Handle &&
       passage[l]->FirstStation == FStation[k]->Handle)
      break;
  if(l == passages)
    {
    if(l == maxpassages)
      {
      maxpassages *= 2;
      TMetroPassage **p = new TMetroPassage *[maxpassages];
      memcpy(p,passage,sizeof(TMetroPassage *) * l);
      delete passage;
      passage = p;
      }
    passage[l] = new TMetroPassage(this,(HPASSAGE) MAKELONG(passages,mode),mode);
    passage[l]->FirstStation = FStation[FSelStation]->Handle;
    passage[l]->SecondStation = FStation[k]->Handle;
    if(mode == ptTrain)
      {
      if(passage[l]->Line == hNull)
        passage[l]->Line = FStation[FSelStation]->Line;
      if(j)
        passage[l]->FirstTime = value.SubString(j + 1, value.Length() - j);
      }
    else
      {
      if(j)
        {
        passage[l]->FirstTime = value.SubString(j + 1, value.Length() - j);
        passage[l]->SecondTime = value.SubString(j + 1, value.Length() - j);
        }
      }
    passages++;
    }
  else
    {
    if(mode == ptTrain)
      {
      if(j)
        passage[l]->SecondTime = value.SubString(j + 1, value.Length() - j);
      else
        passage[l]->SecondTime = passage[l]->FirstTime;
      }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMetroData::RefreshDate()
{
  unsigned int i;
  for(i = 0; i < FStations; i++)
    FStation[i]->SetCaptionByDate(FDate);
  for(i = 0; i < FWalkPassages; i++)
    FWalkPassage[i]->SetCaptionByDate(FDate);
  for(i = 0; i < FTrainPassages; i++)
    FTrainPassage[i]->SetCaptionByDate(FDate);
}
//---------------------------------------------------------------------------
void __fastcall TMetroData::FindInit()
{
  FFndLine = -1;
  FFndStation = -1;
  FFndTrain = -1;
  FFndWalk = -1;
}
//---------------------------------------------------------------------------
HLINE __fastcall TMetroData::FindLine()
{
  FFndLine++;
  if(FFndLine == FLines)
    {
    FFndLine = -1;
    return hNull;
    }
  return FLine[FFndLine]->Handle;
}
//---------------------------------------------------------------------------
HSTATION __fastcall TMetroData::FindStation(HLINE line)
{
  if(line != FLine[FFndLine]->Handle)
    return hNull;
  do
    {
    FFndStation++;
    if(FFndStation == FStations)
      {
      FFndStation = -1;
      return hNull;
      }
    } while (FStation[FFndStation]->Line != line);
  return FStation[FFndStation]->Handle;
}
//---------------------------------------------------------------------------
HPASSAGE __fastcall TMetroData::FindTrainPsg(HLINE line)
{
  if(line != FLine[FFndLine]->Handle)
    return hNull;
  do
    {
    FFndTrain++;
    if(FFndTrain == FTrainPassages)
      {
      FFndTrain = -1;
      return hNull;
      }
    } while (FTrainPassage[FFndTrain]->Line != line);
  return FTrainPassage[FFndTrain]->Handle;
}
//---------------------------------------------------------------------------
HPASSAGE __fastcall TMetroData::FindWalkPsg()
{
  FFndWalk++;
  if(FFndWalk == FWalkPassages)
    {
    FFndWalk = -1;
    return hNull;
    }
  return FWalkPassage[FFndWalk]->Handle;
}
//---------------------------------------------------------------------------
TColor __fastcall TMetroData::GetColor(HLINE line)
{
  unsigned int i = LOWORD(line);
  if(i >= FLines || FLine[i]->Handle != line)
    {
    i = FFndLine;
    if(FLine[i]->Handle != line)
      for(i = 0; i < FLines; i++)
        if(FLine[i]->Handle == line)
          break;
    if(i == FLines)
      return clBlack;
    }
  return FLine[i]->Color;
}
//---------------------------------------------------------------------------
void __fastcall TMetroData::SetDate(TDate value)
{
  if(FDate == value)
    return;
  FDate = value;
  RefreshDate();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMetroData::CaptionOf(HANDLE hndl)
{
  unsigned int i;
  switch(HIWORD(hndl))
    {
    case 0:
      for(i = 0; i < FLines; i++)
        if(FLine[i]->Handle == hndl)
          return FLine[i]->Caption;
    case 1:
      for(i = 0; i < FStations; i++)
        if(FStation[i]->Handle == hndl)
          return FStation[i]->Caption;
    case 2:
      for(i = 0; i < FWalkPassages; i++)
        if(FWalkPassage[i]->Handle == hndl)
          return FWalkPassage[i]->Caption;
    case 3:
      for(i = 0; i < FTrainPassages; i++)
        if(FTrainPassage[i]->Handle == hndl)
          return FTrainPassage[i]->Caption;
    }
  return "";
}
//---------------------------------------------------------------------------
TPoint __fastcall TMetroData::CoordsOfStation(HSTATION st)
{
  unsigned int i = StationIndex(st);
  if(i == -1)
    return Point(0,0);
  return Point(FStation[i]->X,FStation[i]->Y);
}
//---------------------------------------------------------------------------
unsigned int __fastcall TMetroData::AngleOfStationName(HSTATION st)
{
  unsigned int i = StationIndex(st);
  if(i == -1)
    return -1;
  switch(FStation[i]->NameAlign)
    {
    case naLeft:
      return 0;
    case naRight:
      return 180;
    case naTop:
      return 270;
    case naBottom:
      return 90;
    case naTopLeft:
      return 315;
    case naTopRight:
      return 225;
    case naBottomLeft:
      return 45;
    case naBottomRight:
      return 135;
    }
  return 360;
}
//---------------------------------------------------------------------------
TRect __fastcall TMetroData::CoordsOfPassage(HPASSAGE psg)
{
  switch(HIWORD(psg))
    {
    case 2:
      return CoordsOf(psg, FFndWalk, FWalkPassage, FWalkPassages);
    case 3:
      return CoordsOf(psg, FFndTrain, FTrainPassage, FTrainPassages);
    }
  return Rect(0,0,0,0);
}
//---------------------------------------------------------------------------
TRect __fastcall TMetroData::CoordsOf(HPASSAGE psg, int fnd, TMetroPassage ** passage, int passages)
{
  int i = PassageIndex(psg, fnd, passage, passages);
  if(i < 0)
    return Rect(0,0,0,0);
  TPoint p1 = CoordsOfStation(passage[i]->FirstStation), p2 = CoordsOfStation(passage[i]->SecondStation);
  return Rect(p1.x, p1.y, p2.x, p2.y);
}
//---------------------------------------------------------------------------
TPassageDirection __fastcall TMetroData::GetPassageDirection(HPASSAGE psg)
{
  switch(HIWORD(psg))
    {
    case 2:
      return GetPassageDirection(psg, FFndWalk, FWalkPassage, FWalkPassages);
    case 3:
      return GetPassageDirection(psg, FFndTrain, FTrainPassage, FTrainPassages);
    }
  return pdNA;
}
//---------------------------------------------------------------------------
TPassageDirection __fastcall TMetroData::GetPassageDirection(HPASSAGE psg, int fnd, TMetroPassage ** passage, int passages)
{
  int i = PassageIndex(psg, fnd, passage, passages);
  if(i < 0)
    return pdNA;
  if(passage[i]->Type == ptWalk)
    return pdWalkTwoDirectional;
  int t1 = passage[i]->FirstTime, t2 = passage[i]->SecondTime;
  if(!t1 && !t2)
    return pdTwoDirectional;//may be none
  if(!t1)
    return pdBackwardOnly;
  if(!t2)
    return pdForwardOnly;
  return pdTwoDirectional;
}
//---------------------------------------------------------------------------
unsigned int __fastcall TMetroData::PassageIndex(HPASSAGE psg, int fnd, TMetroPassage ** passage, int passages)
{
  int i = LOWORD(psg);
  if(i >= passages || passage[i]->Handle != psg)
    {
    i = fnd;
    if(fnd < 0 || passage[fnd]->Handle != psg)
      for(i = 0; i < passages; i++)
        if(passage[i]->Handle == psg)
          break;
    if(i == passages)
      return -1;
    }
  return i;
}
//---------------------------------------------------------------------------
unsigned int __fastcall TMetroData::StationIndex(HSTATION st)
{
  if(HIWORD(st) != 1)
    return -1;
  unsigned int i = LOWORD(st);
  if(i >= FStations || FStation[i]->Handle != st)
    {
    i = FFndStation;
    if(FStation[i]->Handle != st)
      for(i = 0; i < FStations; i++)
        if(FStation[i]->Handle == st)
          break;
    if(i == FStations)
      return -1;
    }
  return i;
}
//---------------------------------------------------------------------------
namespace Metrodata
{
  void __fastcall PACKAGE Register()
    {
      TComponentClass classes[1] = {__classid(TMetroData)};
      RegisterComponents("Metro", classes, 0);
    }
}


void __fastcall TMetroData::SetLegendOrient(TOrient value)
{
  if(FLegendOrient != value)
    return;
  FLegendOrient = value;
}

void __fastcall TMetroData::SetLegendCols(int value)
{
  if(FLegendCols == value)
    return;
  FLegendCols = value;
}

void __fastcall TMetroData::SetLegendRows(int value)
{
  if(FLegendRows == value)
    return;
  FLegendRows = value;
}

void __fastcall TMetroData::SetLegendNameAlign(TNameAlign value)
{
  if(FLegendNameAlign == value)
    return;
  if(value != naLeft && value != naRight)
    return;
  FLegendNameAlign = value;
}

bool __fastcall TMetroData::GetPassageStations(HPASSAGE psg, HSTATION & st1, HSTATION & st2)
{
  if(HIWORD(psg) != 3 && HIWORD(psg) != 2)
    return false;
  int i;
  if(HIWORD(psg) == 2)
    {
    i = PassageIndex(psg,LOWORD(psg),FWalkPassage,FWalkPassages);
    st1 = FWalkPassage[i]->FirstStation;
    st2 = FWalkPassage[i]->SecondStation;
    }
  else
    {
    i = PassageIndex(psg,LOWORD(psg),FTrainPassage,FTrainPassages);
    st1 = FTrainPassage[i]->FirstStation;
    st2 = FTrainPassage[i]->SecondStation;
    }
  return true;
}

HLINE __fastcall TMetroData::GetLine(HANDLE obj)
{
  int i;
  switch(HIWORD(obj))
    {
    case 1:
      i = StationIndex(obj);
      if(i >= 0)
        return FStation[i]->Line;
      break;
    case 3:
      i = PassageIndex(obj, LOWORD(obj), FTrainPassage, FTrainPassages);
      if(i >= 0)
        return FTrainPassage[i]->Line;
    }
  return hNull;
}
