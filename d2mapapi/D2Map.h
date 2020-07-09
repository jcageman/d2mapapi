#pragma once


#include "D2Structs.h"

#define ArraySize(x) (sizeof(x) / sizeof(x[0]))

void init(const char* dir);

VOID __fastcall D2OpenFile(CHAR* szFile);
VOID D2OpenFile_ASM();
Level* __fastcall GetLevel(ActMisc *misc, DWORD levelno);
VOID D2CLIENT_InitGameMisc(VOID);

VOID __stdcall ExceptionHandler(VOID);
DWORD D2ClientInterface(VOID);
DWORD D2LAUNCH_QueryInterface(VOID);