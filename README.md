
Checkout X and GLAsteroids in adjecent directories.

Install vcpkg. Then run:

0. cd GLAsteroids
0. vcpkg install --x-manifest-root=. --feature-flags=versions,manifests

Build and install the X project.

Build and install the GLAsteroids project:
0. Start Visual Studio
1. Click 'continue without code'
2. Set configuration to be 'x64-Debug'
3. File > Open > CMake
4. If the 'out' directory is not created automatically (Cmake configuration step) then run: Project > Generate Cache for GLAsteroids
5. Build > Build All
6. Build > Install GLAsteroids

Run GLAsteroids:
0. Select the start up item to be 'GLAsteroids.exe'
1. Debug > Start Debugging
