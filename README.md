# azulejo-gl-asteroids

![GLAsteroids.png](./GLAsteroids.png)

## Demo

https://bit.ly/azulejo-gl-asteroids

## How to Play

- `s` yaw left
- `f` yaw right
- `e` thrust
- `j` fire
- `x` reset

## Prerequisites

Checkout [azulejo-common-lib](https://github.com/miclomba/azulejo-common-lib) adjacent to this package.

### Windows

Download [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/)

### Mac OS

0. Install `XCode`
1. Install `clang` by running: `xcode-select --install`
2. Install build tools: `brew install cmake ninja autoconf automake autoconf-archive libtool pkg-config`
3. FreeGLUT depends on `X11` so install `XQuartz` by running: `brew install --cask xquartz`

#### VSCode

0. Install `VSCode`
1. Install VSCode extensions:

- CMake Tools (by Microsoft)
- C/C++ (by Microsoft)

## Install VCPKG Dependencies

Install [vcpkg](https://github.com/microsoft/vcpkg). Then run:

```
cd azulejo-gl-asteroids
vcpkg install --x-manifest-root=. --feature-flags=versions,manifests
```

## Build

### On Windows (Visual Studio)

0. Start Visual Studio
1. Click `continue without code`
2. Set configuration to be `x64-Debug`
3. `File > Open > CMake`
4. If the 'out' directory is not created automatically (Cmake configuration step) then run: `Project > Configure Cache`
5. `Build > Build All`
6. `Build > Install GLAsteroids`

### On MacOS (VSCode)

0. Run `CMake: Configure` command and use the appropriate compiler:

- Clang (on macOS)
- GCC (on Linux)
- Visual Studio (on Windows)

1. Run `CMake: Build`

## Run

### On Windows (Visual Studio)

0. `Select the Startup Item` to be `GLAsteroids.exe`
1. `Debug > Start Debugging`

### On MacOS (VSCode)

0. Run `Start Debugging` with `GLAsteroids` configuration
