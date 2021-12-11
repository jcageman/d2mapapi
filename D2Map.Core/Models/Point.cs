namespace D2Map.Core.Models
{
    public struct Point
    {
        public Point(uint x, uint y)
        {
            X = x;
            Y = y;
        }

        public uint X { get; set; }

        public uint Y { get; set; }
    }
}
