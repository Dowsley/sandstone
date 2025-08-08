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
- [ ] Properties
  - [ ] Density
    - While I am at it, fix the teleporting water problem.
  - Priority?
  - Friction (chance of moving solid sliding down)?
- [ ] Data-driven approach
  - XML loading like in live-world-engine
- [ ] Better water behavior
  - Dispersion rate
- [ ] Extremely basic UI
  - [ ] Brush size
  - [ ] Element chooser
- [ ] Gases
- [ ] Particle System
- [ ] Rigid Body System
- [ ] Temperatures
