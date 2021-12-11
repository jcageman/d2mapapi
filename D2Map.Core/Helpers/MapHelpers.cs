using D2Map.Core.Models;
using D2Map.Core.Wrapper;
using System.Collections.Generic;
using System.Linq;

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
            var collisionMap = new CollisionMap();
            if (act->pActMisc->RealTombArea != 0)
            {
                collisionMap.TombArea = (Area)act->pActMisc->RealTombArea;
            }

            Level* pLevel = MapDll.GetLevel(act->pActMisc, (uint)area);

            if (pLevel != null)
            {
                if (pLevel->pRoom2First == null)
                {
                    MapDll.InitLevel(pLevel);
                }

                if (pLevel->pRoom2First != null)
                {
                    collisionMap.LevelOrigin = new Point(pLevel->dwPosX * 5, pLevel->dwPosY * 5);
                    int width = (int)pLevel->dwSizeX * 5;
                    int height = (int)pLevel->dwSizeY * 5;
                    collisionMap.Map = new List<List<int>>(height);
                    for (int i = 0; i < height; i++)
                    {
                        collisionMap.Map.Add(new List<int>(Enumerable.Repeat(-1, width)));
                    }

                    for (Room2* pRoom2 = pLevel->pRoom2First; pRoom2 != null; pRoom2 = pRoom2->pRoom2Next)
                    {
                        bool bAdded = false;

                        if (pRoom2->pRoom1 == null)
                        {
                            bAdded = true;
                            MapDll.AddRoomData(act, pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, null);
                        }

                        // levels near
                        for (uint i = 0; i < pRoom2->dwRoomsNear; i++)
                        {
                            if (pLevel->dwLevelNo != pRoom2->pRoom2Near[i]->pLevel->dwLevelNo)
                            {
                                var originX = pRoom2->pRoom2Near[i]->pLevel->dwPosX * 5;
                                var originY = pRoom2->pRoom2Near[i]->pLevel->dwPosY * 5;
                                var origin = new Point(originX, originY);
                                var newLevelWidth = pRoom2->pRoom2Near[i]->pLevel->dwSizeX * 5;
                                var newLevelHeight = pRoom2->pRoom2Near[i]->pLevel->dwSizeY * 5;

                                var levelNumber = pRoom2->pRoom2Near[i]->pLevel->dwLevelNo;
                                var adjacentLevel = new AdjacentLevel { LevelOrigin = origin, Width = (int)newLevelWidth, Height = (int)newLevelHeight };
                                collisionMap.AdjacentLevels.TryAdd(levelNumber.ToString(), adjacentLevel);
                            }
                        }

                        // add collision data
                        if (pRoom2->pRoom1 != null && pRoom2->pRoom1->Coll != null)
                        {
                            var x = pRoom2->pRoom1->Coll->dwPosGameX - collisionMap.LevelOrigin.X;
                            var y = pRoom2->pRoom1->Coll->dwPosGameY - collisionMap.LevelOrigin.Y;
                            var cx = pRoom2->pRoom1->Coll->dwSizeGameX;
                            var cy = pRoom2->pRoom1->Coll->dwSizeGameY;
                            var nLimitX = x + cx;
                            var nLimitY = y + cy;

                            var p = pRoom2->pRoom1->Coll->pMapStart;
                            for (var j = y; j < nLimitY; j++)
                            {
                                for (var i = x; i < nLimitX; i++)
                                {
                                    collisionMap.Map[(int)j][(int)i] = *p++;
                                }
                            }
                        }

                        // add unit data
                        for (PresetUnit* pPresetUnit = pRoom2->pPreset; pPresetUnit != null; pPresetUnit = pPresetUnit->pPresetNext)
                        {
                            // npcs
                            if (pPresetUnit->dwType == unit_type_npc)
                            {
                                var npcX = pRoom2->dwPosX * 5 + pPresetUnit->dwPosX;
                                var npcY = pRoom2->dwPosY * 5 + pPresetUnit->dwPosY;
                                var fileNumber = pPresetUnit->dwTxtFileNo.ToString();
                                if (!collisionMap.Npcs.TryAdd(fileNumber, new List<Point> { new Point(npcX, npcY) }))
                                {
                                    collisionMap.Npcs[fileNumber].Add(new Point(npcX, npcY));
                                }
                            }

                            // objects
                            if (pPresetUnit->dwType == unit_type_object)
                            {
                                var objectX = pRoom2->dwPosX * 5 + pPresetUnit->dwPosX;
                                var objectY = pRoom2->dwPosY * 5 + pPresetUnit->dwPosY;
                                var fileNumber = pPresetUnit->dwTxtFileNo.ToString();
                                if (!collisionMap.Objects.TryAdd(fileNumber, new List<Point> { new Point(objectX, objectY) }))
                                {
                                    collisionMap.Objects[fileNumber].Add(new Point(objectX, objectY));
                                }
                            }

                            // level exits
                            if (pPresetUnit->dwType == unit_type_tile)
                            {
                                for (RoomTile* pRoomTile = pRoom2->pRoomTiles; pRoomTile != null; pRoomTile = pRoomTile->pNext)
                                {
                                    if (*pRoomTile->nNum == pPresetUnit->dwTxtFileNo)
                                    {
                                        var exitX = pRoom2->dwPosX * 5 + pPresetUnit->dwPosX;
                                        var exitY = pRoom2->dwPosY * 5 + pPresetUnit->dwPosY;

                                        var levelNumber = pRoomTile->pRoom2->pLevel->dwLevelNo.ToString();
                                        collisionMap.AdjacentLevels[levelNumber].Exits.Add(new Point(exitX, exitY));
                                    }
                                }
                            }
                        }

                        if (bAdded)
                        {
                            MapDll.RemoveRoomData(act, pLevel->dwLevelNo, pRoom2->dwPosX, pRoom2->dwPosY, null);
                        }
                    }
                }
            }

            return collisionMap;
        }
    }
}
