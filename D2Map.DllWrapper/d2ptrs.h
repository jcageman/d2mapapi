#pragma once
#include <Windows.h>
#include <cstdint>

// Supported Diablo II versions. The numeric value is what GetGameVersion() returns to managed code.
enum D2Version : int32_t {
	D2_UNKNOWN = 0,
	D2_109D = 109,
	D2_113C = 113,
};

// Everything that differs between game versions on the native side.
// Ordinals are export ordinals (resolved with GetProcAddress), rvas are offsets from the module base.
struct D2VersionInfo {
	D2Version version;
	uint32_t d2commonTimestamp;     // PE FileHeader.TimeDateStamp of D2Common.dll, used for detection

	// D2Common.dll ordinals
	uint16_t ordLoadAct;
	uint16_t ordUnloadAct;
	uint16_t ordAddRoomData;
	uint16_t ordRemoveRoomData;
	uint16_t ordGetLevel;           // __fastcall (ActMisc*, levelNo): always allocates a new Level
	uint16_t ordInitLevel;          // __stdcall (Level*)
	uint16_t ordInitDataTables;     // __stdcall (0, 0, 0)
	bool loadActHasDifficulty;      // 1.13c: 9 args incl. difficulty; 1.09d: 8 args, no difficulty

	// Fog.dll ordinals (identical in every known version, kept here for completeness)
	uint16_t ordFogSetAppName;      // 10021 (const char*)
	uint16_t ordFogInitAsync;       // 10101 (1, 0)
	uint16_t ordFogInitAsync2;      // 10089 (1)
	uint16_t ordFogInit;            // 10218 ()

	// D2Win.dll ordinals
	uint16_t ordD2WinLoadMpqs;      // 1.13c 10086 / 1.09d 10037: opens d2data/d2exp/patch mpqs
	uint16_t ordD2WinInitArchives;  // 1.13c 10005 / 1.09d 10171: opens d2char/d2music/... and calls client callback
	uint32_t clientStructCallbackOffset; // offset of the "init" callback pointer inside the struct passed to ordD2WinInitArchives

	// D2Lang.dll ordinal
	uint16_t ordD2LangInit;         // 1.13c 10008 / 1.09d 10000: (0, "ENG", 0)

	// Storm.dll: pointer to the lazily allocated MPQ crypt table, reset to 0 before init
	uint32_t rvaStormMpqHashTable;

	// D2Client.dll (only needed for 1.13c, 0 when unused)
	uint32_t rvaClientInitGameMisc;
	uint32_t rvaClientLoadActCallback1;
	uint32_t rvaClientLoadActCallback2;

	// Struct offsets needed natively (GetLevel walks the level list before allocating)
	uint32_t offMiscLevelFirst;     // ActMisc -> Level* first
	uint32_t offLevelNext;          // Level -> Level* next
	uint32_t offLevelNo;            // Level -> DWORD levelNo
};

// Diablo II 1.13c (D2Common.dll timestamp 0x4B95C439)
static const D2VersionInfo D2_INFO_113C = {
	D2_113C, 0x4B95C439,
	10951, 10868, 10401, 11099, 10207, 10322, 10943, true,
	10021, 10101, 10089, 10218,
	10086, 10005, 0x20D,
	10008,
	0x53120,
	0x4454B, 0x62AA0, 0x62760,
	0x47C, 0x1AC, 0x1D0,
};

// Diablo II 1.09d (D2Common.dll timestamp 0x3C06FDCC)
// Reverse engineered from the 1.09d binaries: see README section "1.09 support".
static const D2VersionInfo D2_INFO_109D = {
	D2_109D, 0x3C06FDCC,
	10038, 10039, 10063, 10064, 10013, 10006, 10576, false,
	10021, 10101, 10089, 10218,
	10037, 10171, 0x20C,
	10000,
	0x3A684,
	0, 0, 0,
	0x0, 0x22C, 0x4,
};

static const D2VersionInfo* const D2_KNOWN_VERSIONS[] = { &D2_INFO_113C, &D2_INFO_109D };
