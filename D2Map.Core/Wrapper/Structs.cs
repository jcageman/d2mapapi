namespace D2Map.Core.Wrapper
{
    // The game structures differ per Diablo II version, so they are only used as opaque pointer types.
    // All field access goes through D2Offsets.Current (see D2Offsets.cs).
    public struct Act { }
    public struct ActMisc { }
    public struct Level { }
    public struct Room1 { }
    public struct Room2 { }
    public struct CollMap { }
    public struct PresetUnit { }
    public struct RoomTile { }
}
