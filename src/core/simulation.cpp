//
// Created by João Dowsley on 14/07/25.
//

#include "simulation.h"

Simulation::Simulation(const int width, const int height)
{
    _width = width;
    _height = height;

    _palette[static_cast<size_t>(CellType::EMPTY)] = { 0, 0, 0, 0 };
    _palette[static_cast<size_t>(CellType::SAND)]  = BEIGE;
    _palette[static_cast<size_t>(CellType::WATER)] = SKYBLUE;
    _palette[static_cast<size_t>(CellType::OIL)]   = BLACK;
    _palette[static_cast<size_t>(CellType::FIRE)]  = RED;

    _cells.resize(width * height, CellType::EMPTY);
}

Simulation::Simulation(const Point &size) : Simulation(size.X, size.Y) { }

void Simulation::Step()
{
    // TODO: implement simulation logic here
}

CellType Simulation::GetTypeAt(const int x, const int y) const
{
    return _cells[y * _width + x];
}

void Simulation::FillRenderBuffer(Color *dst) const
{
    const int total = _width * _height;
    for (int i = 0; i < total; ++i) {
        dst[i] = _palette[static_cast<size_t>(_cells[i])];
    }
}


int Simulation::GetWidth() const { return _width; }
int Simulation::GetHeight() const { return _height; }

