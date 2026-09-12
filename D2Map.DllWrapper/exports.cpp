#include "exports.h"
#include "stdafx.h"
#include "d2ptrs.h"
#include <cstring>

// ---- game function signatures -------------------------------------------------------------

// 1.13c: DUNGEON_AllocAct(nAct, nSeed, bClient, pGame, nDifficulty, pMemPool, nTownLevelId, pfnCb1, pfnCb2)
typedef Act* (__stdcall* LoadAct113_t)(uint32_t actno, uint32_t seed, uint32_t bClient, void* pGame, uint32_t difficulty, void* pMempool, uint32_t townLevelId, void* cb1, void* cb2);
// 1.09d: same, but there is no difficulty parameter (maps do not depend on difficulty before 1.10)
typedef Act* (__stdcall* LoadAct109_t)(uint32_t actno, uint32_t seed, uint32_t bClient, void* pGame, void* pMempool, uint32_t townLevelId, void* cb1, void* cb2);
typedef void (__stdcall* UnloadAct_t)(Act* pAct);
typedef Level* (__fastcall* GetLevel_t)(ActMisc* pMisc, uint32_t levelNo);
typedef void (__stdcall* InitLevel_t)(Level* pLevel);
typedef void (__stdcall* RoomData_t)(Act* pAct, int32_t levelId, int32_t x, int32_t y, Room1* pRoom);
typedef uint32_t (__stdcall* InitDataTables_t)(uint32_t, uint32_t, uint32_t);

typedef void (__fastcall* FogSetAppName_t)(const char* szProg);
typedef uint32_t (__fastcall* FogInitAsync_t)(uint32_t, uint32_t);
typedef uint32_t (__fastcall* FogInitAsync2_t)(uint32_t);
typedef uint32_t (__fastcall* FogInit_t)(void);
typedef uint32_t (__fastcall* D2WinLoadMpqs_t)(void);
typedef uint32_t (__fastcall* D2WinInitArchives_t)(uint32_t, uint32_t, uint32_t, void* pClientStruct);
typedef uint32_t (__fastcall* D2LangInit_t)(uint32_t, const char* szLang, uint32_t);

// ---- state ----------------------------------------------------------------------------------

static const D2VersionInfo* g_info = nullptr;
static uint32_t g_d2commonTimestamp = 0;

static struct {
	LoadAct113_t loadAct113;
	LoadAct109_t loadAct109;
	UnloadAct_t unloadAct;
	RoomData_t addRoomData;
	RoomData_t removeRoomData;
	GetLevel_t getLevel;
	InitLevel_t initLevel;
	InitDataTables_t initDataTables;
	FogSetAppName_t fogSetAppName;
	FogInitAsync_t fogInitAsync;
	FogInitAsync2_t fogInitAsync2;
	FogInit_t fogInit;
	D2WinLoadMpqs_t d2winLoadMpqs;
	D2WinInitArchives_t d2winInitArchives;
	D2LangInit_t d2langInit;
} g_fn;

// D2Client pieces (1.13c only)
static uint32_t g_clientInitGameMisc = 0;
static void* g_clientLoadActCb1 = nullptr;
static void* g_clientLoadActCb2 = nullptr;

// Fake "client" structure handed to D2Win: dword 0 must be non-zero, and the callback pointer sits at a
// version specific offset (0x20D in 1.13c, 0x20C in 1.09d).
static uint8_t g_clientStruct[0x400];

// ---- helpers --------------------------------------------------------------------------------

static HMODULE GetOrLoad(const char* dll) {
	HMODULE hmod = GetModuleHandleA(dll);
	if (!hmod)
		hmod = LoadLibraryA(dll);
	return hmod;
}

static FARPROC Ordinal(const char* dll, uint16_t ordinal) {
	HMODULE hmod = GetOrLoad(dll);
	if (!hmod)
		return nullptr;
	return GetProcAddress(hmod, (LPCSTR)(uintptr_t)ordinal);
}

static uint8_t* Rva(const char* dll, uint32_t rva) {
	HMODULE hmod = GetOrLoad(dll);
	if (!hmod)
		return nullptr;
	return (uint8_t*)hmod + rva;
}

static uint32_t ModuleTimestamp(HMODULE hmod) {
	auto dos = (const IMAGE_DOS_HEADER*)hmod;
	if (dos->e_magic != IMAGE_DOS_SIGNATURE)
		return 0;
	auto nt = (const IMAGE_NT_HEADERS*)((const uint8_t*)hmod + dos->e_lfanew);
	if (nt->Signature != IMAGE_NT_SIGNATURE)
		return 0;
	return nt->FileHeader.TimeDateStamp;
}

static const D2VersionInfo* DetectVersion(uint32_t timestamp) {
	for (auto* info : D2_KNOWN_VERSIONS)
		if (info->d2commonTimestamp == timestamp)
			return info;
	return nullptr;
}

template <typename T>
static bool Resolve(T& fn, const char* dll, uint16_t ordinal) {
	fn = (T)Ordinal(dll, ordinal);
	return fn != nullptr;
}

static bool ResolveAll(const D2VersionInfo* info) {
	bool ok = true;
	if (info->loadActHasDifficulty)
		ok &= Resolve(g_fn.loadAct113, "D2Common.dll", info->ordLoadAct);
	else
		ok &= Resolve(g_fn.loadAct109, "D2Common.dll", info->ordLoadAct);
	ok &= Resolve(g_fn.unloadAct, "D2Common.dll", info->ordUnloadAct);
	ok &= Resolve(g_fn.addRoomData, "D2Common.dll", info->ordAddRoomData);
	ok &= Resolve(g_fn.removeRoomData, "D2Common.dll", info->ordRemoveRoomData);
	ok &= Resolve(g_fn.getLevel, "D2Common.dll", info->ordGetLevel);
	ok &= Resolve(g_fn.initLevel, "D2Common.dll", info->ordInitLevel);
	ok &= Resolve(g_fn.initDataTables, "D2Common.dll", info->ordInitDataTables);
	ok &= Resolve(g_fn.fogSetAppName, "Fog.dll", info->ordFogSetAppName);
	ok &= Resolve(g_fn.fogInitAsync, "Fog.dll", info->ordFogInitAsync);
	ok &= Resolve(g_fn.fogInitAsync2, "Fog.dll", info->ordFogInitAsync2);
	ok &= Resolve(g_fn.fogInit, "Fog.dll", info->ordFogInit);
	ok &= Resolve(g_fn.d2winLoadMpqs, "D2Win.dll", info->ordD2WinLoadMpqs);
	ok &= Resolve(g_fn.d2winInitArchives, "D2Win.dll", info->ordD2WinInitArchives);
	ok &= Resolve(g_fn.d2langInit, "D2Lang.dll", info->ordD2LangInit);
	if (info->rvaClientInitGameMisc) {
		g_clientInitGameMisc = (uint32_t)Rva("D2Client.dll", info->rvaClientInitGameMisc);
		g_clientLoadActCb1 = Rva("D2Client.dll", info->rvaClientLoadActCallback1);
		g_clientLoadActCb2 = Rva("D2Client.dll", info->rvaClientLoadActCallback2);
		ok &= g_clientInitGameMisc != 0 && g_clientLoadActCb1 != nullptr && g_clientLoadActCb2 != nullptr;
	}
	else {
		g_clientInitGameMisc = 0;
		g_clientLoadActCb1 = nullptr;
		g_clientLoadActCb2 = nullptr;
	}
	return ok;
}

// Jumps into the tail of D2Client's game-misc initialisation (1.13c only). The tail expects
// ecx/ebp/esi/edi to have been pushed by the function prologue it belongs to.
#if defined(_MSC_VER)
static void __declspec(naked) D2CLIENT_InitGameMisc(void)
{
	__asm
	{
		PUSH ECX
		PUSH EBP
		PUSH ESI
		PUSH EDI
		JMP DWORD PTR [g_clientInitGameMisc]
	}
}
#else
static void __attribute__((naked)) D2CLIENT_InitGameMisc() {
	asm volatile (
		"push %%ecx\n"
		"push %%ebp\n"
		"push %%esi\n"
		"push %%edi\n"
		"jmp *%0\n"
		:
	: "m"(g_clientInitGameMisc)
		);
}
#endif

static uint32_t D2ClientInterface() {
	return *(uint32_t*)g_clientStruct;
}

// ---- exports --------------------------------------------------------------------------------

int32_t GetGameVersion() {
	return g_info ? g_info->version : D2_UNKNOWN;
}

uint32_t GetD2CommonTimestamp() {
	return g_d2commonTimestamp;
}

bool Initialize(const wchar_t* path) {
	wchar_t szPath[260] = { 0 };
	GetCurrentDirectoryW(260, szPath);
	if (path[0] != 0 && path[lstrlenW(path) - 1] != '\\') {
		wchar_t gameDir[260];
		lstrcpyW(gameDir, path);
		lstrcatW(gameDir, L"\\");
		SetCurrentDirectoryW(gameDir);
	}
	else {
		SetCurrentDirectoryW(path);
	}

	bool result = false;
	g_info = nullptr;
	memset(&g_fn, 0, sizeof(g_fn));
	memset(g_clientStruct, 0, sizeof(g_clientStruct));

	do {
		HMODULE common = GetOrLoad("D2Common.dll");
		if (!common)
			break;
		g_d2commonTimestamp = ModuleTimestamp(common);
		const D2VersionInfo* info = DetectVersion(g_d2commonTimestamp);
		if (!info || !ResolveAll(info))
			break;
		g_info = info;

		uint32_t* hashTable = (uint32_t*)Rva("Storm.dll", info->rvaStormMpqHashTable);
		if (!hashTable)
			break;
		*hashTable = 0;

		*(uint32_t*)g_clientStruct = 1;
		*(uint32_t*)(g_clientStruct + info->clientStructCallbackOffset) = (uint32_t)D2ClientInterface;

		g_fn.fogSetAppName("D2");
		g_fn.fogInitAsync(1, 0);
		g_fn.fogInitAsync2(1);
		if (!g_fn.fogInit())
			break;

		if (!g_fn.d2winLoadMpqs() || !g_fn.d2winInitArchives(0, 0, 0, g_clientStruct))
			break;

		g_fn.d2langInit(0, "ENG", 0);

		if (!g_fn.initDataTables(0, 0, 0))
			break;

		if (g_clientInitGameMisc)
			D2CLIENT_InitGameMisc();

		result = true;
	} while (false);

	if (!result)
		g_info = nullptr;
	SetCurrentDirectoryW(szPath);
	return result;
}

Level* GetLevel(ActMisc* misc, uint32_t levelno) {
	if (!g_info || !misc)
		return nullptr;
	// The game's GetLevel always allocates, so look for an existing level first.
	uint8_t* pLevel = *(uint8_t**)((uint8_t*)misc + g_info->offMiscLevelFirst);
	for (; pLevel; pLevel = *(uint8_t**)(pLevel + g_info->offLevelNext))
		if (*(uint32_t*)(pLevel + g_info->offLevelNo) == levelno)
			return (Level*)pLevel;
	return g_fn.getLevel(misc, levelno);
}

void InitLevel(Level* pLevel) {
	if (g_info && pLevel)
		g_fn.initLevel(pLevel);
}

void AddRoomData(Act* pAct, int32_t levelid, int32_t xpos, int32_t ypos, Room1* pRoom) {
	if (g_info)
		g_fn.addRoomData(pAct, levelid, xpos, ypos, pRoom);
}

void RemoveRoomData(Act* pAct, int32_t levelid, int32_t xpos, int32_t ypos, Room1* pRoom) {
	if (g_info)
		g_fn.removeRoomData(pAct, levelid, xpos, ypos, pRoom);
}

Act* LoadAct(uint32_t actno, uint32_t seed, uint32_t difficulty, uint32_t TownLevelId) {
	if (!g_info)
		return nullptr;
	if (g_info->loadActHasDifficulty)
		return g_fn.loadAct113(actno, seed, TRUE, nullptr, difficulty, nullptr, TownLevelId, g_clientLoadActCb1, g_clientLoadActCb2);
	// 1.09: no difficulty parameter; the room callbacks are optional (null-checked by D2Common) and only
	// used by the real client for its automap, so we do not need D2Client at all.
	return g_fn.loadAct109(actno, seed, TRUE, nullptr, nullptr, TownLevelId, g_clientLoadActCb1, g_clientLoadActCb2);
}

void UnloadAct(Act* pAct) {
	if (g_info && pAct)
		g_fn.unloadAct(pAct);
}
