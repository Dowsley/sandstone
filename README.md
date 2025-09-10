## Sandstone

Falling sand simulation using Raylib and vcpkg for dependency management.

## Building

### Prerequisites
- CMake 3.21+
- C++20 compiler
- vcpkg (cloned to `../vcpkg/`)

### Quick Build
```bash
./build.sh
```

### Manual Build
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

### Running
```bash
./build/sandstone
```

## TODO
- [X] Basic sim
- [X] Rewrite on better design pattern
  - [X] Hierarchy of elements
  - [X] Flyweight
  - [X] Solids
  - [X] Liquid
- [X] Brush/sizes
- [X] Main Application Class
- [X] Better build system/dependency handler
- [X] Basic Input handler
- [X] Gases
- [X] Density
- [X] Basic text on UI
- [ ] Controls expanded
  - [X] Q and E to switch elements
  - [X] Scroll wheel to change brush size
  - [X] RMB as eraser
  - [ ] Tab to switch between brush shapes (show this mode as text on screen)
- [ ] Friction
  - (chance of moving solid sliding down)
- [ ] Data-driven approach
  - XML loading like in live-world-engine
- [ ] No more raw pointers (complete refactor)
- [ ] Better water behavior
  - Dispersion rate
- [ ] Extremely basic UI
  - [ ] Element chooser (from list)
  - [ ] Brush visualization
- [ ] Particle System
- [ ] Rigid Body System
- [ ] Temperatures
