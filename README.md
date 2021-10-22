# d2mapapi
Diablo II map rest API to retrieve the map layout per area given a map seed

A diablo 2 1.13C installation is required, but this API should work for all versions of diablo 2. Do note that for 1.09 and lower, higher difficulties do not generate larger maps, hence when interfacing with the API, always use difficulty = 0 (normal).

## Installation

Download & install vcpkg https://github.com/microsoft/vcpkg (don't forget to add to path)

Dependencies should now be installed and included for you when building from source.

If required, manually install and include these dependences:
```
vcpkg install boost-uuid
vcpkg install restinio
vcpkg install json-dto
```

## Usage

d2mapapi.exe DIABLO2_PATH \[ADDRESS_OVERRIDE\]

Examples:
```
d2mapapi.exe "C:\Diablo II1.13c"
d2mapapi.exe "C:\Diablo II1.13c" "0.0.0.0"
```

Starts the webserver

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
CMD ["wine", "d2mapapi.exe", "/app/game", "0.0.0.0"]
```

[Download the latest release](https://github.com/jcageman/d2mapapi/releases)

Place the d2mapapi folder the dockerfile in the same folder, and navigate their via command line then run `docker build -t d2mapapi .`

Once that completes run `docker run d2mapapi`
## API

POST localhost:8080/sessions/ -- starts a new session (difficulty 0,1 or 2 (2 = hell))
```
{
    "difficulty" : 2, 
    "mapid" : 405387119
}
```
DELETE localhost:8080/sessions/{{sessionId}} -- cleans up the session (every new game is a session)

GET localhost:8080/sessions/{{sessionId}}/areas/35  -- retrieves the map information for catacombs 2 (area 35)
