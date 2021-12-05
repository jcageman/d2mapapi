using D2Map.Core.Helpers;
using D2Map.Core.Wrapper;
using System;

namespace D2Map.Core.Models
{
    public unsafe class Session : IDisposable
    {
        public Session(uint mapId, Difficulty difficulty)
        {
            Acts = new Wrapper.Act*[5];
            MapId = mapId;
            Difficulty = difficulty;
        }

        public uint MapId { get; private set; }

        public Difficulty Difficulty { get; private set; }

        private readonly unsafe Wrapper.Act*[] Acts;

        public CollisionMap GetMap(Area area)
        {
            var act = MapHelpers.GetAct(area);
            var actIndex = (int)act;
            if (Acts[actIndex] == null)
            {
                Acts[actIndex] = MapDll.LoadAct((uint)actIndex, MapId, (uint)Difficulty, MapHelpers.ActLevels[actIndex]);
            }
            return MapHelpers.BuildCollissionMap(Acts[actIndex], area);
        }

        public void Dispose()
        {
            foreach (var act in Acts)
            {
                if (act != null)
                {
                    MapDll.UnloadAct(act);
                }
            }
        }
    }
}
