#include "exports.h"
#include "stdafx.h"
#define _DEFINE_VARS
#include "d2ptrs.h"

d2client_struct D2Client;

uint32_t GetDllOffset(const char* dll, int32_t offset) {
    auto hmod = GetModuleHandleA(dll);
    if (!hmod)
        hmod = LoadLibraryA(dll);
    if (!hmod)
        return NULL;
    if (offset < 0)
        return (uint32_t)GetProcAddress(hmod, (LPCSTR)(-offset));
    return ((uint32_t)hmod) + offset;
}

uint32_t GetDllOffset(int32_t num) {
    static const char* dlls[] = { "D2Client.DLL", "D2Common.DLL", "D2Gfx.DLL", "D2Lang.DLL",
                                 "D2Win.DLL", "D2Net.DLL", "D2Game.DLL", "D2Launch.DLL", "Fog.DLL", "BNClient.DLL",
                                 "Storm.DLL", "D2Cmp.DLL" };
    if ((num & 0xff) > 12)
        return NULL;
    return GetDllOffset(dlls[num & 0xff], num >> 8);
}

bool setup_game_dlls() {
    PVOID ptrsToLoad[] = {
        &D2CLIENT_InitGameMisc_I,
        &p_STORM_MPQHashTable,
        &D2CLIENT_LoadAct_1,
        &D2CLIENT_LoadAct_2,
        &D2COMMON_AddRoomData,
        &D2COMMON_RemoveRoomData,
        &D2COMMON_GetLevel,
        &D2COMMON_InitLevel,
        &D2COMMON_LoadAct,
        &D2COMMON_UnloadAct,
        &FOG_10021,
        &FOG_10101,
        &FOG_10089,
        &FOG_10218,
        &D2WIN_10086,
        &D2WIN_10005,
        &D2LANG_10008,
        &D2COMMON_InitDataTables,
    };
    DWORD ptrSaved[sizeof(ptrsToLoad) / sizeof(PVOID)] = {};
    for (auto i = 0u; i < sizeof(ptrsToLoad) / sizeof(PVOID); ++i)
        ptrSaved[i] = *(DWORD*)ptrsToLoad[i];
    bool result = true;
    for (auto* ptr : ptrsToLoad) {
        auto* p = (DWORD*)ptr;
        auto offset = GetDllOffset(*p);
        if (!offset) { result = false; break; }
        *p = offset;
    }
    if (!result)
        for (auto i = 0u; i < sizeof(ptrsToLoad) / sizeof(PVOID); ++i)
            *(DWORD*)ptrsToLoad[i] = ptrSaved[i];
    return result;
}

#if defined(_MSC_VER)
void __declspec(naked) D2CLIENT_InitGameMisc(void)
{
    __asm
    {
        PUSH ECX
        PUSH EBP
        PUSH ESI
        PUSH EDI
        JMP D2CLIENT_InitGameMisc_I
        RETN
    }
}
#else
void __attribute__((naked)) D2CLIENT_InitGameMisc() {
    asm volatile (
        "push %%ecx\n"
        "push %%ebp\n"
        "push %%esi\n"
        "push %%edi\n"
        "jmp *%0\n"
        "ret"
        :
    : "r"(D2CLIENT_InitGameMisc_I)
        );
}
#endif

uint32_t D2ClientInterface() {
    return D2Client.dwInit;
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

	memset(&D2Client, 0, sizeof(d2client_struct));
    if (!setup_game_dlls())
        return false;

    *p_STORM_MPQHashTable = 0;
    D2Client.dwInit = 1;
    D2Client.fpInit = (DWORD)D2ClientInterface;

    FOG_10021("D2");
    FOG_10101(1, 0);
    FOG_10089(1);

    if (!FOG_10218())
        return false;

    if (!D2WIN_10086() || !D2WIN_10005(0, 0, 0, &D2Client))
        return false;

    D2LANG_10008(0, "ENG", 0);

    if (!D2COMMON_InitDataTables(0, 0, 0))
        return false;

    D2CLIENT_InitGameMisc();

    SetCurrentDirectoryW(szPath);
    return true;
}

Level* GetLevel(ActMisc* misc, uint32_t levelno) {
    for (Level* pLevel = misc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel)
        if (pLevel->dwLevelNo == levelno)
            return pLevel;
    return D2COMMON_GetLevel(misc, levelno);
}

void InitLevel(Level* pLevel) {
    return D2COMMON_InitLevel(pLevel);
}

void AddRoomData(Act* pAct, int32_t levelid, int32_t xpos, int32_t ypos, Room1* pRoom) {
    return D2COMMON_AddRoomData(pAct, levelid, xpos, ypos, pRoom);
}

void RemoveRoomData(Act* pAct, int32_t levelid, int32_t xpos, int32_t ypos, Room1* pRoom) {
    return D2COMMON_RemoveRoomData(pAct, levelid, xpos, ypos, pRoom);
}

Act* LoadAct(uint32_t actno, uint32_t seed, uint32_t difficulty, uint32_t TownLevelId) {
    return D2COMMON_LoadAct(actno, seed, TRUE, FALSE, difficulty, NULL, TownLevelId, D2CLIENT_LoadAct_1, D2CLIENT_LoadAct_2);
}

void UnloadAct(Act* pAct) {
    return D2COMMON_UnloadAct(pAct);
}