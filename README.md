# azulejo-gl-asteroids

![GLAsteroids.gif](./resources/GLAsteroids.gif)

## How to Play

- `s` yaw left
- `f` yaw right
- `e` thrust
- `j` fire
- `x` reset

## Prerequisites

Checkout [azulejo-common-lib](https://github.com/miclomba/azulejo-common-lib) then configure, build, and install it.

__NOTE__: You can uninstall via: `azulejo-common-lib/uninstall.sh`

__NOTE__: Source requires `C++20` compilers.

### VSCode

0. Install `VSCode`
1. Install VSCode extensions:

- CMake Tools (by Microsoft)
- C/C++ (by Microsoft)

### Qt Creator

0. Install the open source [Qt Creator](https://www.qt.io/development/download-qt-installer-oss) in `~/Qt`. You will need a developer account to download.

__NOTE__: Qt supported version is `6.11`. If you download a later version you may need to update the `CMakeLists.txt` file to reflect the new version number.

### VCPKG

Install [vcpkg](https://github.com/microsoft/vcpkg) adjacent to this repository directory.

### Mac OS

0. Install `XCode`
1. Install `clang` by running: `xcode-select --install`
2. Install build tools: `./scripts/macos_install_dependencies.sh`
3. Istall dependencies: `./scripts/vcpkg_install.sh`
4. Copy settings `cp .vscode/settings.mac.json .vscode/settings.json`
5. Copy launch settings `cp .vscode/launch.mac.json .vscode/launch.json`

### Ubuntu

0. Install dependencies: `./scripts/linux_install_dependencies.sh`
1. Copy settings `cp .vscode/settings.linux.json .vscode/settings.json`
2. Copy launch settings `cp .vscode/launch.linux.json .vscode/launch.json`

## Build

### VSCode (macOS | Ubuntu)

0. Run `CMake: Configure` command and use the appropriate compiler:

- Clang (on macOS)
- GCC (on Linux)

1. Run `CMake: Build`

### Qt Creator (macOS | Ubuntu)

0. Run `Build > Run qmake`.
1. Run `Build > Build All Projects`.

## Run

### VSCode (macOS | Ubuntu)

0. Run `Start Debugging` with `GLAsteroids` configuration

### Qt Creator (macOS | Ubuntu)

0. In the `Projects` tab:

- Under `Build & Run`:
  - Choose the executable path in the `build` folder.
- In the `Manage Kits`:
  - Select a debugger (e.g. `gdb`)

1. Run `Debug > Start Debugging`
