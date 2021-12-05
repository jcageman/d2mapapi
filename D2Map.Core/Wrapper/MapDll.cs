using System.Runtime.InteropServices;

namespace D2Map.Core.Wrapper
{
    internal static class MapDll
    {
        [DllImport("D2Map.DllWrapper.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        internal extern static bool Initialize(string path);

        [DllImport("D2Map.DllWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static unsafe Level* GetLevel(ActMisc* misc, uint levelno);

        [DllImport("D2Map.DllWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static unsafe void InitLevel(Level* pLevel);

        [DllImport("D2Map.DllWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static unsafe void AddRoomData(Act* pAct, uint levelid, uint xpos, uint ypos, Room1* pRoom);

        [DllImport("D2Map.DllWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static unsafe void RemoveRoomData(Act* pAct, uint levelid, uint xpos, uint ypos, Room1* pRoom);

        [DllImport("D2Map.DllWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static unsafe Act* LoadAct(uint actno, uint seed, uint difficulty, uint TownLevelId);

        [DllImport("D2Map.DllWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        internal extern static unsafe void UnloadAct(Act* pAct);

        [DllImport("kernel32.dll")]
        internal extern static uint GetLastError();
    }
}
