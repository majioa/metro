//---------------------------------------------------------------------------

#ifndef MetroDataH
#define MetroDataH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE EWrongOwner;
class TMetroObject;
class TMetroStation;
class TMetroPassage;
class TMetroLine;
class PACKAGE TMetroData;

#define _ER_OK_                                 0
#define _ER_NO_MORE_WALK_PASSAGES_             -1
#define _ER_NO_MORE_TRAIN_PASSAGES_            -2
#define _ER_PASSAGE_ALREADY_EXIST_             -3
#define _ER_OUT_OF_NAMES_                      -4
#define _ER_STATION_ALREADY_EXIST_             -5
#define _ER_STATION_NOT_FOUND_                 -6
#define _ER_IN_PROJECT_TIMES_ALREADY_EXIST_    -7
#define _ER_WORK_TIMES_ALREADY_EXIST_          -8
#define _ER_ERROR_IN_FILE                      -9
#define _ER_UNKNOWN_FILE_FORMAT_              -10
#define _ER_METRO_FILE_NOT_FOUND_             -11
#define _ER_METRO_FILE_CANNOT_BE_OPEN_        -12

#define _ABSENT_STATION_             (HSTATION) 0
#define _WRONG_HANDLE_                (HANDLE) -1

#define _DEFAULT_WALK_PASSAGES_PER_STATION_     4
#define _DEFAULT_TRAIN_PASSAGES_PER_STATION_    2
#define _DEFAULT_MAX_STATIONS_PER_LINE_        32
#define _DEFAULT_MAX_STATIONS_                256
#define _DEFAULT_MAX_WALK_PASSAGES_            32
#define _DEFAULT_MAX_TRAIN_PASSAGES_          256
#define _DEFAULT_MAX_LINES_                    16
#define _DEFAULT_MAX_WORK_TIMES_                4
#define _DEFAULT_MAX_IN_PROJECT_TIMES_          4

DECLARE_HANDLE(HSTATION);
DECLARE_HANDLE(HPASSAGE);
DECLARE_HANDLE(HLINE);


enum TPassageType {ptWalk = 2, ptTrain = 3};
enum TMetroEnterMode {memChart, memLine, memStation, memWalkPsg, memTrainPsg, memIgnore};
enum TNameAlign {naAuto, naLeft, naTop, naBottom, naRight, naTopLeft, naTopRight, naBottomLeft, naBottomRight};
enum TPassageDirection {pdWalkTwoDirectional, pdTwoDirectional , pdForwardOnly, pdBackwardOnly, pdDifferentWays, pdNA};
enum TOrient {otHorz, otVert};

typedef void __fastcall (__closure *TLoadProgressEvent)(System::TObject* Sender, int Percent);

const HANDLE hNull = (void *) -1;

class EWrongOwner : public Exception
{
public:
  __fastcall EWrongOwner(const AnsiString Msg) : Sysutils::Exception(Msg) { }
  __fastcall EWrongOwner(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
  __fastcall EWrongOwner(int Ident, Extended Dummy) : Sysutils::
    Exception(Ident, Dummy) { }

  __fastcall EWrongOwner(int Ident, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Ident, Args, Args_Size) { }
  __fastcall EWrongOwner(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
  __fastcall EWrongOwner(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception
    (Msg, Args, Args_Size, AHelpContext) { }

  __fastcall EWrongOwner(int Ident, int AHelpContext) : Sysutils::
    Exception(Ident, AHelpContext) { }
  __fastcall EWrongOwner(int Ident, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception
    (Ident, Args, Args_Size, AHelpContext) { }
};


class TMetroObject : public TPersistent
{
private:
  HANDLE FHandle;
//  TDate FNull;//data form which any time is calculated
  AnsiString FCaption;
  AnsiString FName;
  TDate FNameSetted;
  TDate FNameExpired;
  TDate *FInProject;
  unsigned char FInProjects;
  unsigned char FMaxInProjects;
  TDate *FWork;
  unsigned char FWorks;
  unsigned char FMaxWorks;
  TStringList *FCaptions;
  TMetroData * FOwner;
  unsigned int FFindIndex;
  AnsiString FSubStr;
  TNameAlign FNameAlign;
  int __fastcall FindSubStr(AnsiString str, AnsiString substr);
protected:
  int FLastError;
  __property AnsiString Caption  = { read=FCaption };
  __property AnsiString Name  = { read=FName, write=FName };
public:
  __fastcall TMetroObject(TMetroData* Owner, HANDLE hndl);
  __fastcall ~TMetroObject();
  __property int LastError  = { read=FLastError };
  __property TMetroData * Owner  = { read=FOwner };
  virtual void __fastcall Clear();
  virtual bool __fastcall IsInProject(TDate date);
  virtual bool __fastcall IsWorking(TDate date);
  virtual bool __fastcall IsExpired(TDate date);
  virtual bool __fastcall IsNameTrue(TDate date);
  virtual bool __fastcall SetCaptionByDate(TDate date);
  virtual void __fastcall AddCaption(AnsiString caption);
  void __fastcall AddProjectDate(AnsiString);
  void __fastcall AddWorkDate(AnsiString);
  void __fastcall AddTimes(AnsiString str, TDate * date, unsigned char &dates, unsigned char &maxdates, bool fl);
  __property TNameAlign NameAlign  = { read=FNameAlign, write=FNameAlign };
  __property HANDLE Handle  = { read=FHandle };
};


class TMetroStation : public TMetroObject
{
private:
  HLINE FLine;
  HPASSAGE *FWalkPassage;
  unsigned int FWalkPassages;
  unsigned int FMaxWalkPassages;   //default = 4
  HPASSAGE *FTrainPassage;
  unsigned int FTrainPassages;
  unsigned int FMaxTrainPassages;   //default = 2
  int FX;
  int FY;
  unsigned int FWalkIndex;
  unsigned int FTrainIndex;
  TTime FStandTime;
  bool __fastcall AssignPassage(HPASSAGE psg, HPASSAGE * &p, unsigned int &passages, unsigned int &maxpassages);
protected:
public:
  __fastcall TMetroStation(TMetroData* Owner, HSTATION id);
  __fastcall ~TMetroStation();
  void __fastcall Clear();
  bool __fastcall AssignPassage(HPASSAGE psg, TPassageType type);
  HPASSAGE __fastcall GetFirstWalk();
  HPASSAGE __fastcall GetNextWalk();
  HPASSAGE __fastcall GetFirstTrain();
  HPASSAGE __fastcall GetNextTrain();
  __property Caption;
  __property Name;
  __property HLINE Line  = { read=FLine, write=FLine };
  __property HPASSAGE * WalkPassage  = { read=FWalkPassage };
  __property unsigned int WalkPassages  = { read=FWalkPassages };
  __property HPASSAGE * TrainPassage  = { read=FTrainPassage };
  __property unsigned int TrainPassages  = { read=FTrainPassages };
  __property int X  = { read=FX, write=FX };
  __property int Y  = { read=FY, write=FY };
  __property TTime StandTime  = { read=FStandTime, write=FStandTime };
};


class TMetroPassage : public TMetroObject
{
private:
  TPassageType FType;
  HSTATION FFirstStation;
  HSTATION FSecondStation;
  HLINE FLine;
  bool FShowClockwise;
  TTime FFirstTime;
  TTime FSecondTime;
/*  TTime FForwardTime;
  TTime FBackwardTime;*/
  //for train passage type its are used as forward time and backward time
  //forward time is a time has passed during doing train from FFirstStation to SecondStation
  //backward time is a time has passed during doing train from SecondStation to FFirstStation
  //for walk passage type its are used as lower and upper edges of walk
  //from FFirstStation to SecondStation or from SecondStation to FFirstStation
protected:
public:
  __fastcall TMetroPassage(TMetroData* Owner, HPASSAGE id, TPassageType type);
  __fastcall ~TMetroPassage();
  void __fastcall Clear();
  __property Name;
  __property Caption;
  __property TPassageType Type  = { read=FType };
  __property HSTATION FirstStation  = { read=FFirstStation, write=FFirstStation };
  __property HSTATION SecondStation  = { read=FSecondStation, write=FSecondStation };
  __property bool ShowClockwise  = { read=FShowClockwise, write=FShowClockwise };
  __property TTime FirstTime  = { read=FFirstTime, write=FFirstTime };
  __property TTime SecondTime  = { read=FSecondTime, write=FSecondTime };
  __property HLINE Line  = { read=FLine, write=FLine };
/*  __property TTime ForwardTime  = { read=FForwardTime, write=FForwardTime };
  __property TTime BackwardTime  = { read=FBackwardTime, write=FBackwardTime };*/
};


class TMetroLine : public TMetroObject
{
private:
  TColor FColor;
  HSTATION *FStation;
  unsigned int FStations;
  unsigned int FMaxStations;
protected:
public:
  __fastcall TMetroLine(TMetroData* Owner, HLINE id);
  __fastcall ~TMetroLine();
  void __fastcall Clear();
  void __fastcall AddStation(HSTATION stat);
  bool __fastcall RemoveStation(HSTATION);
  __property Name;
  __property Caption;
  __property TColor Color  = { read=FColor, write=FColor };
  __property HSTATION *Station  = { read=FStation };
  __property unsigned int Stations  = { read=FStations };
};


class PACKAGE TMetroData : public TComponent
{
private:
  AnsiString FCaption;
  AnsiString FVisibleName;
  TMetroStation ** FStation;
  unsigned int FStations;
  TMetroPassage ** FWalkPassage;
  unsigned int FWalkPassages;
  TMetroPassage ** FTrainPassage;
  unsigned int FTrainPassages;
  TMetroLine ** FLine;
  unsigned int FLines;
  TMetroLine *FSelLine;
  unsigned int FSelStation;
  TMetroPassage *FSelWalk;
  TMetroPassage *FSelTrain;
  AnsiString FMetroFile;
  AnsiString FBackground;
  TColor FColor;
  bool FAutoLoad;
  unsigned int FMaxStations;
  unsigned int FMaxWalkPassages;
  unsigned int FMaxTrainPassages;
  unsigned int FMaxLines;
  int FLastError;
  int FChartWidth;
  int FChartHeight;
  AnsiString FChartAuthor;
  TNotifyEvent FOnAfterLoading;
  TNotifyEvent FOnLoadComplete;
  TLoadProgressEvent FOnLoadProgress;
  bool FEmpty;
  unsigned int FFndLine;
  unsigned int FFndStation;
  unsigned int FFndTrain;
  unsigned int FFndWalk;
  TColor FStationBorderColor;
  TColor FStationBlankColor;
  TColor FMetroObjectNameColor;
  TColor FWalkPsgColor;
  bool FStationBorder;
  bool FStationBlank;
  TDate FDate;
  TOrient FLegendOrient;
  int FLegendCols;
  int FLegendRows;
  TNameAlign FLegendNameAlign;
  void __fastcall SetMetroFile(AnsiString value);
  void __fastcall SetBackground(AnsiString value);
  void __fastcall SetColor(TColor color);
  bool __fastcall LoadText(TFileStream * file);
  bool __fastcall LoadBinary(TFileStream * file);
  void __fastcall CalcPassage(AnsiString value, unsigned int & passages, unsigned int & maxpassages, TMetroPassage ** passage, TPassageType mode);
  TRect __fastcall CoordsOf(HPASSAGE psg, int fnd, TMetroPassage ** passage, int passages);
  TPassageDirection __fastcall GetPassageDirection(HPASSAGE psg, int fnd, TMetroPassage ** passage, int passages);
  unsigned int __fastcall PassageIndex(HPASSAGE psg, int fnd, TMetroPassage ** passage, int passages);
  unsigned int __fastcall StationIndex(HSTATION st);
  void __fastcall SetDate(TDate value);
  void __fastcall RefreshDate();
  void __fastcall SetLegendOrient(TOrient value);
  void __fastcall SetLegendCols(int value);
  void __fastcall SetLegendRows(int value);
  void __fastcall SetLegendNameAlign(TNameAlign value);
//  bool __fastcall SaveText(TFileStream * file);
//  bool __fastcall SaveBinary(TFileStream * file);
protected:
public:
  __fastcall TMetroData(TComponent* Owner);
  void __fastcall Clear();
  bool __fastcall LoadMetroFile(AnsiString file);
  __fastcall ~TMetroData();
  void __fastcall FindInit();
  HLINE __fastcall FindLine();
  HSTATION __fastcall FindStation(HLINE);
  HPASSAGE __fastcall FindTrainPsg(HLINE line);
  HPASSAGE __fastcall FindWalkPsg();
  TColor __fastcall GetColor(HLINE);
  TPoint __fastcall CoordsOfStation(HSTATION);
  TRect __fastcall CoordsOfPassage(HPASSAGE);
  unsigned int __fastcall AngleOfStationName(HSTATION st);
  TPassageDirection __fastcall GetPassageDirection(HPASSAGE);
  AnsiString __fastcall CaptionOf(HANDLE);
  bool __fastcall GetPassageStations(HPASSAGE psg, HSTATION & st1, HSTATION & st2);
  HLINE __fastcall GetLine(HANDLE st);
  __property TMetroStation ** Station  = { read=FStation };
  __property unsigned int Stations  = { read=FStations };
  __property TMetroPassage ** WalkPassage  = { read=FWalkPassage };
  __property unsigned int WalkPassages  = { read=FWalkPassages };
  __property TMetroPassage ** TrainPassage  = { read=FTrainPassage };
  __property unsigned int TrainPassages  = { read=FTrainPassages };
  __property TMetroLine ** Line  = { read=FLine };
  __property unsigned int Lines  = { read=FLines };
  __property int LastError  = { read=FLastError };
  __property int ChartWidth  = { read=FChartWidth };
  __property int ChartHeight  = { read=FChartHeight };
  __property AnsiString ChartAuthor  = { read=FChartAuthor };
  __property bool Empty  = { read=FEmpty };
  __property TNotifyEvent OnAfterLoading  = { read=FOnAfterLoading, write=FOnAfterLoading };
  __property TColor StationBorderColor  = { read=FStationBorderColor, write=FStationBorderColor };
  __property TColor StationBlankColor  = { read=FStationBlankColor, write=FStationBlankColor };
  __property TColor WalkPsgColor  = { read=FWalkPsgColor, write=FWalkPsgColor };
  __property bool StationBorder  = { read=FStationBorder, write=FStationBorder, default=false };
  __property bool StationBlank  = { read=FStationBlank, write=FStationBlank, default=false };
  __property TDate Date  = { read=FDate, write=SetDate };
__published:
  __property AnsiString VisibleName  = { read=FVisibleName,write=FVisibleName};
  __property AnsiString Caption  = { read=FCaption, write=FCaption };
  __property bool AutoLoad  = { read=FAutoLoad, write=FAutoLoad };
  __property AnsiString MetroFile  = { read=FMetroFile, write=SetMetroFile };
  __property AnsiString Background  = { read=FBackground, write=SetBackground };
  __property TColor Color  = { read=FColor, write=SetColor };
  __property TNotifyEvent OnLoadComplete  = { read=FOnLoadComplete, write=FOnLoadComplete };
  __property TLoadProgressEvent OnLoadProgress  = { read=FOnLoadProgress, write=FOnLoadProgress };
  __property TColor MetroObjectNameColor  = { read=FMetroObjectNameColor, write=FMetroObjectNameColor };
  __property TOrient LegendOrient  = { read=FLegendOrient, write=SetLegendOrient };
  __property int LegendCols  = { read=FLegendCols, write=SetLegendCols };
  __property int LegendRows  = { read=FLegendRows, write=SetLegendRows };
  __property TNameAlign LegendNameAlign  = { read=FLegendNameAlign, write=SetLegendNameAlign };
};
//---------------------------------------------------------------------------
#endif
