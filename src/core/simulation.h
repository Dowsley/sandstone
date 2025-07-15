//
// Created by João Dowsley on 14/07/25.
//

#ifndef SIMULATION_H
#define SIMULATION_H

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

    void step();
    
    CellType get_type_at(int x, int y) const;
    void fill_render_buffer(Color *dst) const;

    int get_width() const;
    int get_height() const;

private:
    int _width;
    int _height;

    std::vector<CellType> _cells;
    std::array<Color, static_cast<size_t>(CellType::COUNT)> _palette;
};

#endif //SIMULATION_H
