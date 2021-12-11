using System;
using System.Runtime.InteropServices;

namespace D2Map.Core.Wrapper
{
    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct Act
    {
        [FieldOffset(0xC)] public uint MapSeed;
        [FieldOffset(0x14)] public uint ActId;
        [FieldOffset(0x48)] public ActMisc* pActMisc;
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct Level
    {
        [FieldOffset(0x10)] public Room2* pRoom2First;
        [FieldOffset(0x1C)] public uint dwPosX;
        [FieldOffset(0x20)] public uint dwPosY;
        [FieldOffset(0x24)] public uint dwSizeX;
        [FieldOffset(0x28)] public uint dwSizeY;
        [FieldOffset(0x1AC)] public Level* pNextLevel;
        [FieldOffset(0x1B4)] public ActMisc* pMisc;
        [FieldOffset(0x1D0)] public uint dwLevelNo;
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct ActMisc
    {
        [FieldOffset(0x94)] public uint RealTombArea;
        [FieldOffset(0x46C)] public Act* pAct;
        [FieldOffset(0x47C)] public Level* pLevelFirst;
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct CollMap
    {
        [FieldOffset(0x00)] public uint dwPosGameX;
        [FieldOffset(0x04)] public uint dwPosGameY;
        [FieldOffset(0x08)] public uint dwSizeGameX;
        [FieldOffset(0x0C)] public uint dwSizeGameY;
        [FieldOffset(0x10)] public uint dwPosRoomX;
        [FieldOffset(0x14)] public uint dwPosRoomY;
        [FieldOffset(0x18)] public uint dwSizeRoomX;
        [FieldOffset(0x1C)] public uint dwSizeRoomY;
        [FieldOffset(0x20)] public ushort* pMapStart;
        [FieldOffset(0x22)] public ushort* pMapEnd;
    };

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct Room1
    {
        [FieldOffset(0x00)] public Room1** pRoomsNear;
        [FieldOffset(0x10)] public Room2* pRoom2;
        [FieldOffset(0x20)] public CollMap* Coll;
        [FieldOffset(0x24)] public uint numRoomsNear;
        [FieldOffset(0x4C)] public uint dwPosX;
        [FieldOffset(0x50)] public uint dwPosY;
        [FieldOffset(0x54)] public uint dwSizeX;
        [FieldOffset(0x58)] public uint dwSizeY;
        [FieldOffset(0x74)] public IntPtr pUnitFirst;
        [FieldOffset(0x7C)] public Room1* pRoomNext;
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct PresetUnit
    {
        [FieldOffset(0x04)] public uint dwTxtFileNo;
        [FieldOffset(0x08)] public uint dwPosX;
        [FieldOffset(0x0C)] public PresetUnit* pPresetNext;
        [FieldOffset(0x14)] public uint dwType;
        [FieldOffset(0x18)] public uint dwPosY;
    };

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct RoomTile
    {
        [FieldOffset(0x00)] public Room2* pRoom2;
        [FieldOffset(0x04)] public RoomTile* pNext;
        [FieldOffset(0x10)] public uint* nNum;
    };

    [StructLayout(LayoutKind.Explicit)]
    public unsafe struct Room2
    {
        [FieldOffset(0x08)] public Room2** pRoom2Near;
        [FieldOffset(0x24)] public Room2* pRoom2Next;
        [FieldOffset(0x30)] public Room1* pRoom1;
        [FieldOffset(0x2C)] public uint dwRoomsNear;
        [FieldOffset(0x34)] public uint dwPosX;
        [FieldOffset(0x38)] public uint dwPosY;
        [FieldOffset(0x3C)] public uint dwSizeX;
        [FieldOffset(0x40)] public uint dwSizeY;
        [FieldOffset(0x4C)] public RoomTile* pRoomTiles;
        [FieldOffset(0x58)] public Level* pLevel;
        [FieldOffset(0x5C)] public PresetUnit* pPreset;
    }
}
