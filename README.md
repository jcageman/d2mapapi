# d2mapapi
Diablo II map rest API to retrieve the map layout per area given a map seed for diablo II 1.13C

Note: A diablo 2 1.13C installation is required!

## Installation

Download & install vcpkg https://github.com/microsoft/vcpkg

```
vcpkg install boost-uuid
vcpkg install restinio
vcpkg install json-dto
```

## Usage

d2mapapi.exe "C:\Diablo II1.13c"

Starts the webserver

## API

POST localhost:8080/sessions/ -- starts a new session (difficulty 0,1 or 2 (2 = hell)
```
{
    "difficulty" : 2, 
    "mapid" : 405387119
}
```
DELETE localhost:8080/sessions/{{sessionId}} -- cleans up the session (every new game is a session)

GET localhost:8080/sessions/{{sessionId}}/areas/35  -- retrieves the map information for catacombs 2 (area 35)
