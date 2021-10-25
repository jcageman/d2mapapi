# d2mapapi
Diablo II map rest API to retrieve the map layout per area given a map seed

A diablo 2 1.13C installation is required, but this API should work for all versions of diablo 2. Do note that for 1.09 and lower, higher difficulties do not generate larger maps, hence when interfacing with the API, always use difficulty = 0 (normal).

## Installation

First install [vcpkg](https://github.com/microsoft/vcpkg)

### Windows
```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```

### Unix
```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.sh
```

The binary will be located here `.\vcpkg\vcpkg`, make sure to include the absolute path to this in your system path environment variable and restart your shell. Once done setup a vcpkg integration by running:

```
vcpkg integrate install
```

If you are using Visual Studio to build this project, vcpkg dependencies will be automatically installed and included to this repository. If not, you will need to manually fetch and include the following packages:

```
vcpkg install boost-uuid
vcpkg install restinio
vcpkg install json-dto
```

You should now be able to build d2mapapi.sln

## Usage

Syntax:
```
d2mapapi.exe {pathToDiablo2} [optionalArguments]
```

### Optional Arguments
    * -i or --ip
        specify an IP address to use instead of localhost. ex: `-i 192.168.0.1` OR `--ip=192.168.0.1`";
    * -p or --port
        specify n port address to use instead of the default 8080. ex: `-p 8080` OR `--port=8080`";
    * -h or --help
        display usage information

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
CMD ["wine", "d2mapapi.exe", "/app/game", "--ip=0.0.0.0"]
```

[Download the latest release](https://github.com/rmilejcz/d2mapapi/releases)

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
