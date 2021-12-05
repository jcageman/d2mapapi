using D2Map.Core.Models;

namespace D2Map.Core
{
    public interface IMapService
    {
        CollisionMap GetCollisionMap(uint mapId, Difficulty difficulty, Area area);
    }
}
