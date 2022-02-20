# d2mapapi
Diablo II map rest API to retrieve the map layout per area given a map id.

A diablo 2 1.13C installation is required, but the result returned by this API should work for all versions of diablo 2, since the map generation has stayed the same across all verisons.

Note: or 1.09 and lower, higher difficulties do not generate larger maps, hence when interfacing with the API, always use difficulty normal.

## Installation

This project assumes you have VS2022 installed with .NET 6.0 and C++ workloads.

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

