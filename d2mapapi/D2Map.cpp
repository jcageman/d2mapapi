#include "D2Map.h"
#include "D2Structs.h"
#include <fstream>
#include <iostream>

#include "D2Ptrs.h"
#include "Offset.h"


d2client_struct D2Client;

char D2_DIR[MAX_PATH] = "";


void init(const char* dir)
{
	sprintf_s(D2_DIR, sizeof(D2_DIR), "%s\\", dir);

	char GAME[MAX_PATH] = "";
	sprintf_s(GAME, sizeof(GAME), "%s\\game.exe", dir);
	std::ifstream ifs(GAME, std::ifstream::in);
	if (!ifs)
	{
		std::cout << "Incorrect Folder Name" << std::endl;
		exit(1);
	}
	ifs.close();

	CHAR szPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szPath);
	memset(&D2Client, NULL, sizeof(d2client_struct));
	SetCurrentDirectory(D2_DIR);
	DefineOffsets();
	
	*p_STORM_MPQHashTable = NULL;
	D2Client.dwInit = 1;
	D2Client.fpInit = (DWORD)D2ClientInterface;
	
	FOG_10021("D2");
	FOG_10101(1, NULL);
	FOG_10089(1);
	
	if (!FOG_10218())
	{
		std::cout << "Game Server Initialize Failed!" << std::endl;
		ExitProcess( NULL );
	}

	if (!D2WIN_10086() || !D2WIN_10005( NULL, NULL, NULL, &D2Client )) {
		std::cout << "Couldn't load Diablo 2 MPQ files. Please make sure you have a full install of Diablo II and copy the D2XMUSIC.MPQ and D2XVIDEO.MPQ from the Expansion CD" << std::endl;
		ExitProcess( NULL );
	}
	
	D2LANG_10008(0,"ENG",0);

	if (!D2COMMON_InitDataTables(0,0,0))
	{
		std::cout << "Couldn't initialize sqptDataTable!" << std::endl;
		ExitProcess(NULL);
	}

	D2CLIENT_InitGameMisc();

	SetCurrentDirectory(szPath);

}

Level* __fastcall GetLevel(ActMisc *misc, DWORD levelno)
{
	for (Level *pLevel = misc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel)
		if (pLevel->dwLevelNo == levelno)
			return pLevel;
	
	return D2COMMON_GetLevel(misc, levelno);
}

VOID __declspec(naked) D2CLIENT_InitGameMisc(VOID)
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

DWORD D2ClientInterface(VOID)
{
	return D2Client.dwInit;
}
