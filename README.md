# d2mapapi
Diablo II map rest API to retrieve the map layout per area given a map seed

A diablo 2 1.13C installation is required, but this API should work for all versions of diablo 2. Do note that for 1.09 and lower, higher difficulties do not generate larger maps, hence when interfacing with the API, always use difficulty = 0 (normal).

## Installation

Download & install vcpkg https://github.com/microsoft/vcpkg

```
vcpkg install restinio
vcpkg install json-dto
```

## Usage

d2mapapi.exe "C:\Diablo II1.13c"

Starts the webserver

## API

The following requests obtains the map information for a given mapId, difficulty (0,1 or 2) and areaId (e.g. 101 is Durance Of Hate Level 2)
```
GET localhost:8080/maprequests?mapid=1530537122&difficulty=0&areaid=101
```
