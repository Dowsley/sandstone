//
// Created by João Dowsley on 14/07/25.
//

#include "simulation.h"

Simulation::Simulation(const int width, const int height)
{
    _width = width;
    _height = height;

    _palette[static_cast<size_t>(CellType::EMPTY)] = { 0, 0, 0, 0 };
    _palette[static_cast<size_t>(CellType::SAND)]  = YELLOW;
    _palette[static_cast<size_t>(CellType::STONE)] = GRAY;
    _palette[static_cast<size_t>(CellType::WATER)] = BLUE;
    _palette[static_cast<size_t>(CellType::OIL)]   = PURPLE;
    _palette[static_cast<size_t>(CellType::FIRE)]  = RED;

    _cells.resize(width * height, CellType::EMPTY);
}

Simulation::Simulation(const Point &size) : Simulation(size.x, size.y) { }

void Simulation::step()
{
    // Make a copy of the current grid
    std::vector<CellType> next = _cells;

    // Process from bottom to top so falling feels natural
    for (int y = _height - 1; y >= 0; --y) {
        for (int x = 0; x < _width; ++x) {
            const int idx  = flatten_coords(x, y);
            const auto type = _cells[idx];
            Point pos{x, y};

            if (type == CellType::SAND) {
                // 1) Try straight down
                Point below = pos + DOWN;
                if (below.y < _height &&
                    _cells[flatten_coords(below)] == CellType::EMPTY) {
                    next[flatten_coords(below)] = CellType::SAND;
                    next[idx] = CellType::EMPTY;
                    continue;
                }

                // 2) Try down-left only if left is free
                Point left = pos + LEFT;
                if (left.x >= 0 &&
                    _cells[flatten_coords(left)] == CellType::EMPTY) {
                    Point down_left = pos + DOWN + LEFT;
                    if (down_left.y < _height &&
                        _cells[flatten_coords(down_left)] == CellType::EMPTY) {
                        next[flatten_coords(down_left)] = CellType::SAND;
                        next[idx] = CellType::EMPTY;
                        continue;
                    }
                }

                // 3) Try down-right only if right is free
                Point right = pos + RIGHT;
                if (right.x < _width &&
                    _cells[flatten_coords(right)] == CellType::EMPTY) {
                    Point down_right = pos + DOWN + RIGHT;
                    if (down_right.y < _height &&
                        _cells[flatten_coords(down_right)] == CellType::EMPTY) {
                        next[flatten_coords(down_right)] = CellType::SAND;
                        next[idx] = CellType::EMPTY;
                        continue;
                    }
                }

                // Otherwise it stays put
            }
      else if (type == CellType::WATER)
            {
                // 1) Fall straight down if empty
                Point below = pos + DOWN;
                if (below.y < _height &&
                    _cells[flatten_coords(below)] == CellType::EMPTY)
                {
                    next[flatten_coords(below)] = CellType::WATER;
                    next[idx] = CellType::EMPTY;
                    continue;
                }

                // 2) Try down-left diagonal
                Point down_left = pos + DOWN + LEFT;
                if (down_left.y < _height &&
                    down_left.x >= 0 &&
                    _cells[flatten_coords(down_left)] == CellType::EMPTY)
                {
                    next[flatten_coords(down_left)] = CellType::WATER;
                    next[idx] = CellType::EMPTY;
                    continue;
                }

                // 3) Try down-right diagonal
                Point down_right = pos + DOWN + RIGHT;
                if (down_right.y < _height &&
                    down_right.x < _width &&
                    _cells[flatten_coords(down_right)] == CellType::EMPTY)
                {
                    next[flatten_coords(down_right)] = CellType::WATER;
                    next[idx] = CellType::EMPTY;
                    continue;
                }

                // 4) Finally, slide sideways (randomizing to avoid bias)
                Point left_pos  = pos + LEFT;
                Point right_pos = pos + RIGHT;
                bool can_left  = (left_pos.x  >= 0)         && _cells[flatten_coords(left_pos)]  == CellType::EMPTY;
                bool can_right = (right_pos.x < _width)    && _cells[flatten_coords(right_pos)] == CellType::EMPTY;

                if (can_left || can_right)
                {
                    bool go_left = can_left && (!can_right || GetRandomValue(0,1) == 0);
                    Point dest   = go_left ? left_pos : right_pos;
                    next[flatten_coords(dest)] = CellType::WATER;
                    next[idx] = CellType::EMPTY;
                    continue;
                }

                // 5) Otherwise, no move: water accumulates on water
            }
        }
    }

    // Commit the new state
    _cells = std::move(next);
}

void Simulation::set_type_at(const int x, const int y, const CellType type)
{
    _cells[flatten_coords(x, y)] = type;
}

CellType Simulation::get_type_at(const int x, const int y) const
{
    return _cells[flatten_coords(x, y)];
}

void Simulation::fill_render_buffer(Color *dst) const
{
    const int total = _width * _height;
    for (int i = 0; i < total; ++i) {
        dst[i] = _palette[static_cast<size_t>(_cells[i])];
    }
}

int Simulation::get_width() const { return _width; }
int Simulation::get_height() const { return _height; }

int Simulation::flatten_coords(const int x, const int y) const
{
    return y * _width + x;
}

int Simulation::flatten_coords(const Point &pos) const
{
    return flatten_coords(pos.x, pos.y);
}

