# d2mapapi
Diablo II map rest API to retrieve the map layout per area given a map id.

A Diablo II **1.13c** or **1.09d** installation is required. The version is detected automatically from the
installation's `D2Common.dll`, and the same API works for both. The level layout (collision map, level bounds,
exits) is identical between versions for the same map seed; see [1.09 support](#109-support) for what differs.

Note: for 1.09 and lower, higher difficulties do not generate larger maps. When a 1.09 installation is used the
`difficulty` parameter is ignored and every difficulty returns the Normal map.

## Installation

This project assumes you have VS2022 installed with .NET 8.0 and C++ workloads.

## Usage Examples
```
./D2Map.Api.exe Diablo2Directory="C:\Diablo II1.13c"
./D2Map.Api.exe --urls https://localhost:8080 Diablo2Directory="C:\Diablo II1.13c"
./D2Map.Api.exe --urls https://0.0.0.0:8080 Diablo2Directory="C:\Diablo II1.13c"
```

Note: to run from source use dotnet run or simply start the project from visual studio.

## Running in Docker

This requires [Docker Desktop](https://www.docker.com/products/docker-desktop)

Example Dockerfile:
```
FROM tianon/wine
ENV WINEARCH=win32
ENV WINEDEBUG=-all
# change this to your Diablo 2 installation
COPY [ "C:\Diablo II1.13c", "/app/game" ]
WORKDIR /app
# d2mapapi should be in the same folder as your dockerfile
COPY ./d2mapapi .
EXPOSE 8080
CMD ["wine", "D2Map.Api.exe", "Diablo2Directory=/app/game", "--urls=https://0.0.0.0:8080"]
```

[Download the latest release](https://github.com/jcageman/d2mapapi/releases)

Place the d2mapapi folder the dockerfile in the same folder, and navigate their via command line then run `docker build -t d2mapapi .`

Once that completes run `docker run d2mapapi`
## API

GET https://localhost:5001/maps?mapid=1053646565&area=BloodMoor&difficulty=Normal

GET https://localhost:5001/maps?mapid=1053646565&area=2&difficulty=0

[List Of Areas](/D2Map.Core/Models/Area.cs)


## 1.09 support

The native wrapper (`D2Map.DllWrapper`) detects the game version from the PE timestamp of `D2Common.dll`
(`0x4B95C439` = 1.13c, `0x3C06FDCC` = 1.09d) and picks the matching export ordinals and structure offsets.
The 1.09d values were reverse engineered from the 1.09d binaries and live in two places:

- `D2Map.DllWrapper/d2ptrs.h`: export ordinals (D2Common, D2Win, D2Lang, Fog), the Storm MPQ hash table
  variable and the few offsets the native side needs.
- `D2Map.Core/Wrapper/D2Offsets.cs`: structure offsets (Act, ActMisc, Level, Room1, Room2, CollMap,
  PresetUnit, RoomTile) used to walk the generated level.

Notable differences in 1.09d:

- `LoadAct` has no difficulty parameter (8 arguments instead of 9), so all difficulties share one map.
- The D2Client callbacks are not needed; they are optional in D2Common and only feed the in-game automap.
- NPC ids in the `npcs` result follow the 1.09 `monstats.txt` numbering, which differs from 1.13c for the
  preset spawn placeholders (e.g. 1.13c ids 734-793 are 612-671 in 1.09, 802-836 are 577-611).
  Level bounds, collision data, exits and object ids are the same as 1.13c for the same seed.

Usage is unchanged, just point `Diablo2Directory` at the 1.09d installation:

```
./D2Map.Api.exe Diablo2Directory="C:\Diablo II 1.09"
```
