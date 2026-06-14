# Template for Raylib Web Build for Linux
Template to create web games with [Raylib](https://www.raylib.com). Allows for pushing the build directly to [itch.io](https://itch.io).

## Usage
In `nob.c` define the following macros:
- RAYLIB_DIR: Path to webassembly build of raylib
- HTML_OUTPUT: HTML output for game
- ASYNCIFY: To enable ASYNCIFY feature of emcc
- ITCH_USERNAME: itch.io username
- GAME: Game display name in itch.io

### Dependencies
- gcc or clang
- [emcc](https://emscripten.org/docs/getting_started/downloads.html)
- [python](https://www.python.org/)
- [butler](https://itch.io/docs/butler/) (If you want to push to itch.io)

Usage for C:
```sh
cc -o nob nob.c
./nob -help
```
Usage for C++:
```sh
mv nob.c nob.cpp
mv src/main.c src/main.cpp
cc -x c++ -o nob nob.cpp
./nob -help
```
