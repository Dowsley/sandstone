//
// Created by João Dowsley on 14/07/25.
//

#ifndef SIMULATION_H
#define SIMULATION_H
#include <cstdint>
#include <raylib.h>
#include <vector>

#include "../types/point.h"

enum class CellType : uint8_t {
    EMPTY = 0,
    SAND,
    STONE,
    WATER,
    OIL,
    FIRE,
    COUNT,
};


class Simulation {
public:
    Simulation(int width, int height);
    explicit Simulation(const Point &size);
    ~Simulation() = default;

    void Step();
    
    CellType GetTypeAt(const int x, const int y) const;
    void FillRenderBuffer(Color *dst) const;

    int GetWidth() const;
    int GetHeight() const;

private:
    int _width;
    int _height;

    std::vector<CellType> _cells;
    std::array<Color, static_cast<size_t>(CellType::COUNT)> _palette;
};

#endif //SIMULATION_H
