#include "stdafx.h"

BOOL APIENTRY DllMain(
	HMODULE hmodule, 
	DWORD ul_reason_for_call, 
	LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		DisableThreadLibraryCalls(hmodule);
	return TRUE;
}