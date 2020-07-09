# d2mapapi
d2 map rest API to retrieve the map layout per area given a map seed for diablo 1.13C

Note: A diablo 2 1.13C installation is required!

## Installation

Download vcpkg https://github.com/microsoft/vcpkg

```
vcpkg install boost-uuid
vcpkg install restinio
vcpkg install json-dto
```

## Usage

d2mapapi.exe "C:\Diablo II1.13c"

Starts the webserver

Endpoints:
POST localhost:8080/sessions/ -- starts a new session
```
{
    "difficulty" : 2,
    "mapid" : 405387119
}
```
DELETE localhost:8080/sessions/{{sessionId}} -- cleans up the session (every new game is a session)

GET localhost:8080/sessions/{{sessionId}}/35  -- retrieves the map information for catacombs 2 (area 35)
