using System;

namespace D2Map.Core.Wrapper
{
    public enum GameVersion
    {
        Unknown = 0,
        V109d = 109,
        V113c = 113,
    }

    /// <summary>
    /// Structure offsets of the game's map (DRLG) structures for one Diablo II version.
    /// 1.13c offsets come from the well known community structs (McGod); 1.09d offsets were
    /// reverse engineered from the 1.09d D2Common.dll (see README, section "1.09 support").
    /// </summary>
    public sealed class D2Offsets
    {
        public GameVersion Version { get; private set; }

        /// <summary>Before 1.10 the level layout is the same on every difficulty; LoadAct has no difficulty parameter.</summary>
        public bool MapDependsOnDifficulty { get; private set; }

        // Act
        public int Act_pActMisc;

        // ActMisc (DRLG)
        public int ActMisc_RealTombArea;
        public int ActMisc_pLevelFirst;

        // Level
        public int Level_pRoom2First;
        public int Level_dwPosX;
        public int Level_dwPosY;
        public int Level_dwSizeX;
        public int Level_dwSizeY;
        public int Level_pNextLevel;
        public int Level_dwLevelNo;

        // Room2 (DrlgRoom)
        public int Room2_pRoom2Near;
        public int Room2_pRoom2Next;
        public int Room2_dwRoomsNear;
        public int Room2_pRoom1;
        public int Room2_dwPosX;
        public int Room2_dwPosY;
        public int Room2_dwSizeX;
        public int Room2_dwSizeY;
        public int Room2_pRoomTiles;
        public int Room2_pLevel;
        public int Room2_pPreset;

        // Room1 (active room)
        public int Room1_pRoom2;
        public int Room1_Coll;
        public int Room1_pRoomNext;

        // CollMap
        public int CollMap_dwPosGameX;
        public int CollMap_dwPosGameY;
        public int CollMap_dwSizeGameX;
        public int CollMap_dwSizeGameY;
        public int CollMap_pMapStart;

        // PresetUnit
        public int PresetUnit_dwTxtFileNo;
        public int PresetUnit_dwPosX;
        public int PresetUnit_dwPosY;
        public int PresetUnit_pPresetNext;
        public int PresetUnit_dwType;

        // RoomTile (warp link to an adjacent level)
        public int RoomTile_pRoom2;
        public int RoomTile_pNext;
        public int RoomTile_nNum;   // pointer to the LvlWarp record, first dword is the warp id

        public static readonly D2Offsets V113c = new D2Offsets
        {
            Version = GameVersion.V113c,
            MapDependsOnDifficulty = true,

            Act_pActMisc = 0x48,

            ActMisc_RealTombArea = 0x94,
            ActMisc_pLevelFirst = 0x47C,

            Level_pRoom2First = 0x10,
            Level_dwPosX = 0x1C,
            Level_dwPosY = 0x20,
            Level_dwSizeX = 0x24,
            Level_dwSizeY = 0x28,
            Level_pNextLevel = 0x1AC,
            Level_dwLevelNo = 0x1D0,

            Room2_pRoom2Near = 0x08,
            Room2_pRoom2Next = 0x24,
            Room2_dwRoomsNear = 0x2C,
            Room2_pRoom1 = 0x30,
            Room2_dwPosX = 0x34,
            Room2_dwPosY = 0x38,
            Room2_dwSizeX = 0x3C,
            Room2_dwSizeY = 0x40,
            Room2_pRoomTiles = 0x4C,
            Room2_pLevel = 0x58,
            Room2_pPreset = 0x5C,

            Room1_pRoom2 = 0x10,
            Room1_Coll = 0x20,
            Room1_pRoomNext = 0x7C,

            CollMap_dwPosGameX = 0x00,
            CollMap_dwPosGameY = 0x04,
            CollMap_dwSizeGameX = 0x08,
            CollMap_dwSizeGameY = 0x0C,
            CollMap_pMapStart = 0x20,

            PresetUnit_dwTxtFileNo = 0x04,
            PresetUnit_dwPosX = 0x08,
            PresetUnit_pPresetNext = 0x0C,
            PresetUnit_dwType = 0x14,
            PresetUnit_dwPosY = 0x18,

            RoomTile_pRoom2 = 0x00,
            RoomTile_pNext = 0x04,
            RoomTile_nNum = 0x10,
        };

        public static readonly D2Offsets V109d = new D2Offsets
        {
            Version = GameVersion.V109d,
            MapDependsOnDifficulty = false,

            Act_pActMisc = 0x08,

            ActMisc_RealTombArea = 0x3F4,
            ActMisc_pLevelFirst = 0x00,

            Level_pRoom2First = 0x30,
            Level_dwPosX = 0x20,
            Level_dwPosY = 0x24,
            Level_dwSizeX = 0x28,
            Level_dwSizeY = 0x2C,
            Level_pNextLevel = 0x22C,
            Level_dwLevelNo = 0x04,

            Room2_pRoom2Near = 0xC0,
            Room2_pRoom2Next = 0xE8,
            Room2_dwRoomsNear = 0xC4,
            Room2_pRoom1 = 0xE4,
            Room2_dwPosX = 0x04,
            Room2_dwPosY = 0x08,
            Room2_dwSizeX = 0x0C,
            Room2_dwSizeY = 0x10,
            Room2_pRoomTiles = 0xC8,
            Room2_pLevel = 0x00,
            Room2_pPreset = 0xCC,

            Room1_pRoom2 = 0x38,
            Room1_Coll = 0x34,
            Room1_pRoomNext = 0x7C,

            CollMap_dwPosGameX = 0x08,
            CollMap_dwPosGameY = 0x0C,
            CollMap_dwSizeGameX = 0x10,
            CollMap_dwSizeGameY = 0x14,
            CollMap_pMapStart = 0x00,

            PresetUnit_dwType = 0x00,
            PresetUnit_dwTxtFileNo = 0x04,
            PresetUnit_dwPosX = 0x0C,
            PresetUnit_dwPosY = 0x10,
            PresetUnit_pPresetNext = 0x1C,

            RoomTile_pRoom2 = 0x00,
            RoomTile_nNum = 0x04,
            RoomTile_pNext = 0x14,
        };

        private static D2Offsets _current;

        /// <summary>Offsets of the game version the native wrapper was initialized with.</summary>
        public static D2Offsets Current => _current ?? throw new InvalidOperationException("The map DLL has not been initialized.");

        public static void Select(GameVersion version)
        {
            _current = version switch
            {
                GameVersion.V113c => V113c,
                GameVersion.V109d => V109d,
                _ => throw new NotSupportedException($"Unsupported Diablo II version: {version}"),
            };
        }

        // Raw memory readers used by the map builder.
        public static unsafe uint U32(void* p, int offset) => *(uint*)((byte*)p + offset);
        public static unsafe T* Ptr<T>(void* p, int offset) where T : unmanaged => *(T**)((byte*)p + offset);
        public static unsafe void* RawPtr(void* p, int offset) => *(void**)((byte*)p + offset);
        public static unsafe ushort* U16Ptr(void* p, int offset) => *(ushort**)((byte*)p + offset);
    }
}
