using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace D2Map.Core.Models
{
    public class CollisionMap
    {
        public Point LevelOrigin { get; set; }
        [JsonPropertyName("mapRows")]
        public List<List<int>> Map { get; set; }
        public Dictionary<string, AdjacentLevel> AdjacentLevels { get; set; } = new Dictionary<string, AdjacentLevel>();
        public Dictionary<string, List<Point>> Npcs { get; set; } = new Dictionary<string, List<Point>>();
        public Dictionary<string, List<Point>> Objects { get; set; } = new Dictionary<string, List<Point>>();
        public Area? TombArea { get; set; }
    };
}
