#pragma once
#include <wtypes.h>

struct UnitAny;
struct Room1;
struct Room2;
struct Level;
struct Act;
struct ActMisc;
struct RosterUnit;
struct OverheadMsg;

struct CollMap { //1.13?
	DWORD dwPosGameX;				//0x00
	DWORD dwPosGameY;				//0x04
	DWORD dwSizeGameX;				//0x08
	DWORD dwSizeGameY;				//0x0C
	DWORD dwPosRoomX;				//0x10
	DWORD dwPosRoomY;				//0x14
	DWORD dwSizeRoomX;				//0x18
	DWORD dwSizeRoomY;				//0x1C
	WORD* pMapStart;				//0x20
	WORD* pMapEnd;					//0x22
};

#pragma pack(push, 1)
struct d2client_struct {
	DWORD dwInit;
	BYTE _1[0x209];
	DWORD fpInit;
};

struct RoomTile
{
	Room2* pRoom2;            //0x00
	RoomTile* pNext;          //0x04
	DWORD _2[2];            //0x08
	DWORD* nNum;             //0x10
};

//1.13c - PresetUnit - McGod
struct PresetUnit
{
	DWORD _1;                  //0x00
	DWORD dwTxtFileNo;            //0x04
	DWORD dwPosX;               //0x08
	PresetUnit* pPresetNext;      //0x0C
	DWORD _3;                  //0x10
	DWORD dwType;               //0x14
	DWORD dwPosY;               //0x18
};

//1.13c - Level - McGod
struct Level {
	DWORD _1[4];         //0x00
	Room2* pRoom2First;      //0x10
	DWORD _2[2];         //0x14
	DWORD dwPosX;         //0x1C
	DWORD dwPosY;         //0x20
	DWORD dwSizeX;         //0x24
	DWORD dwSizeY;         //0x28
	DWORD _3[96];         //0x2C
	Level* pNextLevel;      //0x1AC
	DWORD _4;            //0x1B0
	ActMisc* pMisc;         //0x1B4
	DWORD _5[6];         //0x1BC
	DWORD dwLevelNo;      //0x1D0
};

//1.13c - Room2 - McGod
struct Room2 {
	DWORD _1[2];         //0x00
	Room2** pRoom2Near;      //0x08
	DWORD _2[6];         //0x0C
	Room2* pRoom2Next;      //0x24
	DWORD dwRoomFlags;      //0x28
	DWORD dwRoomsNear;      //0x2C
	Room1* pRoom1;         //0x30
	DWORD dwPosX;         //0x34
	DWORD dwPosY;         //0x38
	DWORD dwSizeX;         //0x3C
	DWORD dwSizeY;         //0x40
	DWORD _3;            //0x44
	DWORD dwPresetType;      //0x48
	RoomTile* pRoomTiles;   //0x4C
	DWORD _4[2];         //0x50
	Level* pLevel;         //0x58
	PresetUnit* pPreset;   //0x5C
};

//1.13c - Room1 - McGod   
struct Room1 {
	Room1** pRoomsNear;    //0x00
	DWORD _1[3];         //0x04
	Room2* pRoom2;         //0x10
	DWORD _2[3];         //0x14
	CollMap* Coll;         //0x20
	DWORD dwRoomsNear;      //0x24
	DWORD _3[9];         //0x28
	DWORD dwPosX;         //0x4C
	DWORD dwPosY;         //0x50
	DWORD dwSizeX;         //0x54
	DWORD dwSizeY;         //0x58
	DWORD _4[6];         //0x5C
	UnitAny* pUnitFirst;   //0x74
	DWORD _5;            //0x78
	Room1* pRoomNext;      //0x7C
};


//1.13c - ActMisc - McGod
struct ActMisc {
	DWORD _1[37];         //0x00
	DWORD dwStaffTombLevel; //0x94
	DWORD _2[245];         //0x98
	Act* pAct;            //0x46C
	DWORD _3[3];         //0x470
	Level* pLevelFirst;      //0x47C
};

//1.13c - Act - McGod   
struct Act {
	DWORD _1[3];         //0x00
	DWORD dwMapSeed;      //0x0C
	Room1* pRoom1;         //0x10
	DWORD dwAct;         //0x14
	DWORD _2[12];         //0x18
	ActMisc* pMisc;         //0x48
};
#pragma pack(pop)