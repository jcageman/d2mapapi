#pragma once

#include "D2Structs.h"

#ifdef _DEFINE_VARS

enum {DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2LANG, DLLNO_D2WIN, DLLNO_D2NET, DLLNO_D2GAME, DLLNO_D2LAUNCH, DLLNO_FOG, DLLNO_BNCLIENT, DLLNO_STORM, DLLNO_D2CMP, DLLNO_D2MULTI, DLLNO_D2SOUND};

#define DLLOFFSET(a1,b1) ((DLLNO_##a1)|((b1)<<8))
#define FUNCPTR(d1,v1,t1,t2,o1)	typedef t1 d1##_##v1##_t t2; d1##_##v1##_t *d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define VARPTR(d1,v1,t1,o1)		typedef t1 d1##_##v1##_t;    d1##_##v1##_t *p_##d1##_##v1 = (d1##_##v1##_t *)DLLOFFSET(d1,o1);
#define ASMPTR(d1,v1,o1)			DWORD d1##_##v1 = DLLOFFSET(d1,o1);

#else

#define FUNCPTR(d1,v1,t1,t2,o1)	typedef t1 d1##_##v1##_t t2; extern d1##_##v1##_t *d1##_##v1;
#define VARPTR(d1,v1,t1,o1)		typedef t1 d1##_##v1##_t;    extern d1##_##v1##_t *p_##d1##_##v1;
#define ASMPTR(d1,v1,o1)			extern DWORD d1##_##v1;

#endif
#define _D2PTRS_START	D2CLIENT_InitGameMisc_I

FUNCPTR(D2CLIENT, InitGameMisc_I, VOID __stdcall, (DWORD Dummy1, DWORD Dummy2, DWORD Dummy3), 0x4454B) // Updated
VARPTR(STORM, MPQHashTable, DWORD, 0x53120) // Updated
ASMPTR(D2CLIENT, LoadAct_1, 0x62AA0) // Updated
ASMPTR(D2CLIENT, LoadAct_2, 0x62760) // Updated
FUNCPTR(D2COMMON, AddRoomData, void __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, Room1 * pRoom), -10401)//Updated  // 1.12 -10184
FUNCPTR(D2COMMON, RemoveRoomData, void __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, Room1 * pRoom), -11099)//Updated // 1.12 -11009
FUNCPTR(D2COMMON, GetLevel, Level * __fastcall, (ActMisc *pMisc, DWORD dwLevelNo), -10207)//Updated // 1.12 -11020

FUNCPTR(D2COMMON, InitLevel, void __stdcall, (Level *pLevel), -10322)//Updated // 1.12 -10721
FUNCPTR(D2COMMON, LoadAct, Act* __stdcall, (DWORD ActNumber, DWORD MapId, DWORD Unk, DWORD Unk_2, DWORD Unk_3, DWORD Unk_4, DWORD TownLevelId, DWORD Func_1, DWORD Func_2), 0x3CB30)//Updated 1.13 0x3CB30 // 1.12  0x56780
FUNCPTR(D2COMMON, UnloadAct, VOID __stdcall, (Act* pAct), -10868) //Updated // 1.12 -10710

FUNCPTR(FOG, 10021, VOID __fastcall, (CHAR* szProg), -10021) // 1.12 & 1.13
FUNCPTR(FOG, 10101, DWORD __fastcall, (DWORD _1, DWORD _2), -10101) // 1.12 & 1.13
FUNCPTR(FOG, 10089, DWORD __fastcall, (DWORD _1), -10089) // 1.12 & 1.13
FUNCPTR(FOG, 10218, DWORD __fastcall, (VOID), -10218) // 1.12 & 1.13

FUNCPTR(D2WIN, 10086, DWORD __fastcall, (VOID), -10086) // Updated
FUNCPTR(D2WIN, 10005, DWORD __fastcall, (DWORD _1, DWORD _2, DWORD _3, d2client_struct* pD2Client), -10005) //Updated

FUNCPTR(D2LANG, 10008, DWORD __fastcall, (DWORD _1, CHAR* _2, DWORD _3), -10008) //Updated
FUNCPTR(D2COMMON, InitDataTables, DWORD __stdcall, (DWORD _1, DWORD _2, DWORD _3), -10943)//Updated //1.12 -10797

#define _D2PTRS_END	D2COMMON_InitDataTables

#undef FUNCPTR
#undef VARPTR
#undef ASMPTR