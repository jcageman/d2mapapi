using System.Collections.Generic;

namespace D2Map.Core.Models
{
    public class AdjacentLevel
    {
        public List<Point> Exits { get; set; } = new List<Point>();
        public Point LevelOrigin { get; set; }
        public int Width { get; set; }
        public int Height { get; set; }
    }
}
