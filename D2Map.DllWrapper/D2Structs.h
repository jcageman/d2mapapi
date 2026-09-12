#pragma once
#include <wtypes.h>

// The game structures differ between Diablo II versions, so the native layer only treats them
// as opaque handles. Field access happens through the per-version offset tables:
//   native side: D2VersionInfo in d2ptrs.h (only what GetLevel needs)
//   managed side: D2Offsets in D2Map.Core/Wrapper/D2Offsets.cs
struct Act;
struct ActMisc;
struct Level;
struct Room1;
struct Room2;
