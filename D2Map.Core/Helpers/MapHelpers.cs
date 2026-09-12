using D2Map.Core.Models;
using D2Map.Core.Wrapper;
using System.Collections.Generic;
using System.Linq;
using static D2Map.Core.Wrapper.D2Offsets;

namespace D2Map.Core.Helpers
{
    public static class MapHelpers
    {
        static public uint[] ActLevels = { 1, 40, 75, 103, 109, 137 };

        static private uint unit_type_npc = 1;
        static private uint unit_type_object = 2;
        static private uint unit_type_tile = 5;
        public static Models.Act GetAct(Area area)
        {
            for (uint i = 1; i < 5; ++i)
            {
                if ((int)area < ActLevels[i])
                {
                    return (Models.Act)(i - 1);
                }
            }
            return Models.Act.Act5;
        }

        public unsafe static CollisionMap BuildCollissionMap(Wrapper.Act* act, Area area)
        {
            var o = Current;
            var collisionMap = new CollisionMap();
            ActMisc* pMisc = Ptr<ActMisc>(act, o.Act_pActMisc);
            uint realTombArea = U32(pMisc, o.ActMisc_RealTombArea);
            if (realTombArea != 0)
            {
                collisionMap.TombArea = (Area)realTombArea;
            }

            Level* pLevel = MapDll.GetLevel(pMisc, (uint)area);

            if (pLevel != null)
            {
                if (Ptr<Room2>(pLevel, o.Level_pRoom2First) == null)
                {
                    MapDll.InitLevel(pLevel);
                }

                if (Ptr<Room2>(pLevel, o.Level_pRoom2First) != null)
                {
                    uint levelNo = U32(pLevel, o.Level_dwLevelNo);
                    collisionMap.LevelOrigin = new Point(U32(pLevel, o.Level_dwPosX) * 5, U32(pLevel, o.Level_dwPosY) * 5);
                    int width = (int)U32(pLevel, o.Level_dwSizeX) * 5;
                    int height = (int)U32(pLevel, o.Level_dwSizeY) * 5;
                    collisionMap.Map = new List<List<int>>(height);
                    for (int i = 0; i < height; i++)
                    {
                        collisionMap.Map.Add(new List<int>(Enumerable.Repeat(-1, width)));
                    }

                    for (Room2* pRoom2 = Ptr<Room2>(pLevel, o.Level_pRoom2First); pRoom2 != null; pRoom2 = Ptr<Room2>(pRoom2, o.Room2_pRoom2Next))
                    {
                        bool bAdded = false;
                        uint roomPosX = U32(pRoom2, o.Room2_dwPosX);
                        uint roomPosY = U32(pRoom2, o.Room2_dwPosY);

                        if (Ptr<Room1>(pRoom2, o.Room2_pRoom1) == null)
                        {
                            bAdded = true;
                            MapDll.AddRoomData(act, levelNo, roomPosX, roomPosY, null);
                        }

                        // levels near
                        Room2** pRoomsNear = (Room2**)RawPtr(pRoom2, o.Room2_pRoom2Near);
                        uint roomsNear = U32(pRoom2, o.Room2_dwRoomsNear);
                        for (uint i = 0; i < roomsNear; i++)
                        {
                            Level* pNearLevel = Ptr<Level>(pRoomsNear[i], o.Room2_pLevel);
                            uint nearLevelNo = U32(pNearLevel, o.Level_dwLevelNo);
                            if (levelNo != nearLevelNo)
                            {
                                var originX = U32(pNearLevel, o.Level_dwPosX) * 5;
                                var originY = U32(pNearLevel, o.Level_dwPosY) * 5;
                                var origin = new Point(originX, originY);
                                var newLevelWidth = U32(pNearLevel, o.Level_dwSizeX) * 5;
                                var newLevelHeight = U32(pNearLevel, o.Level_dwSizeY) * 5;

                                var adjacentLevel = new AdjacentLevel { LevelOrigin = origin, Width = (int)newLevelWidth, Height = (int)newLevelHeight };
                                collisionMap.AdjacentLevels.TryAdd(nearLevelNo.ToString(), adjacentLevel);
                            }
                        }

                        // add collision data
                        Room1* pRoom1 = Ptr<Room1>(pRoom2, o.Room2_pRoom1);
                        CollMap* pColl = pRoom1 != null ? Ptr<CollMap>(pRoom1, o.Room1_Coll) : null;
                        if (pColl != null)
                        {
                            var x = U32(pColl, o.CollMap_dwPosGameX) - collisionMap.LevelOrigin.X;
                            var y = U32(pColl, o.CollMap_dwPosGameY) - collisionMap.LevelOrigin.Y;
                            var cx = U32(pColl, o.CollMap_dwSizeGameX);
                            var cy = U32(pColl, o.CollMap_dwSizeGameY);
                            var nLimitX = x + cx;
                            var nLimitY = y + cy;

                            var p = U16Ptr(pColl, o.CollMap_pMapStart);
                            for (var j = y; j < nLimitY; j++)
                            {
                                for (var i = x; i < nLimitX; i++)
                                {
                                    collisionMap.Map[(int)j][(int)i] = *p++;
                                }
                            }
                        }

                        // add unit data
                        for (PresetUnit* pPresetUnit = Ptr<PresetUnit>(pRoom2, o.Room2_pPreset); pPresetUnit != null; pPresetUnit = Ptr<PresetUnit>(pPresetUnit, o.PresetUnit_pPresetNext))
                        {
                            uint type = U32(pPresetUnit, o.PresetUnit_dwType);
                            uint txtFileNo = U32(pPresetUnit, o.PresetUnit_dwTxtFileNo);
                            uint unitX = roomPosX * 5 + U32(pPresetUnit, o.PresetUnit_dwPosX);
                            uint unitY = roomPosY * 5 + U32(pPresetUnit, o.PresetUnit_dwPosY);

                            // npcs
                            if (type == unit_type_npc)
                            {
                                var fileNumber = txtFileNo.ToString();
                                if (!collisionMap.Npcs.TryAdd(fileNumber, new List<Point> { new Point(unitX, unitY) }))
                                {
                                    collisionMap.Npcs[fileNumber].Add(new Point(unitX, unitY));
                                }
                            }

                            // objects
                            if (type == unit_type_object)
                            {
                                var fileNumber = txtFileNo.ToString();
                                if (!collisionMap.Objects.TryAdd(fileNumber, new List<Point> { new Point(unitX, unitY) }))
                                {
                                    collisionMap.Objects[fileNumber].Add(new Point(unitX, unitY));
                                }
                            }

                            // level exits
                            if (type == unit_type_tile)
                            {
                                for (RoomTile* pRoomTile = Ptr<RoomTile>(pRoom2, o.Room2_pRoomTiles); pRoomTile != null; pRoomTile = Ptr<RoomTile>(pRoomTile, o.RoomTile_pNext))
                                {
                                    uint* nNum = Ptr<uint>(pRoomTile, o.RoomTile_nNum);
                                    if (nNum != null && *nNum == txtFileNo)
                                    {
                                        Room2* pTileRoom2 = Ptr<Room2>(pRoomTile, o.RoomTile_pRoom2);
                                        var tileLevelNumber = U32(Ptr<Level>(pTileRoom2, o.Room2_pLevel), o.Level_dwLevelNo).ToString();
                                        if (collisionMap.AdjacentLevels.TryGetValue(tileLevelNumber, out var adjacent))
                                        {
                                            adjacent.Exits.Add(new Point(unitX, unitY));
                                        }
                                    }
                                }
                            }
                        }

                        if (bAdded)
                        {
                            MapDll.RemoveRoomData(act, levelNo, roomPosX, roomPosY, null);
                        }
                    }
                }
            }

            return collisionMap;
        }
    }
}
