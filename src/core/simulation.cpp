#include "simulation.h"
#include <utility> // For std::move

Simulation::Simulation(const int width, const int height)
{
    _width = width;
    _height = height;

    _palette.fill(BLACK);
    _palette[static_cast<size_t>(CellType::EMPTY)] = {0, 0, 0, 0};           // Transparent
    _palette[static_cast<size_t>(CellType::SAND)]  = { 240, 213, 129, 255 }; // Sandy yellow
    _palette[static_cast<size_t>(CellType::STONE)] = GRAY;
    _palette[static_cast<size_t>(CellType::WATER)] = { 60, 128, 242, 255 };  // Blue
    _palette[static_cast<size_t>(CellType::OIL)]   = { 87, 43, 97, 255 };    // Dark Purple
    _palette[static_cast<size_t>(CellType::FIRE)]  = { 255, 90, 0, 255 };    // Orange-Red

    _cells.resize(width * height, CellType::EMPTY);
}

Simulation::Simulation(const Point &size) : Simulation(size.x, size.y) { }

void Simulation::step()
{
    std::vector<CellType> next = _cells;

    // Randomize the horizontal update direction each frame to prevent directional bias in liquids.
    // TODO: Analyze if this is really effective.
    const bool reverse_x = GetRandomValue(0, 1) == 1;

    // Process the grid from bottom to top, which is more stable for falling particles.
    for (int y = _height - 1; y >= 0; --y) {
        for (int i = 0; i < _width; ++i) {
            const int x = reverse_x ? (_width - 1 - i) : i;

            const int idx  = flatten_coords(x, y);
            const auto type = _cells[idx];
            Point pos{x, y};

            // If a particle was already moved into this spot by another particle above it,
            // its original self has been overwritten in `next`, so we skip it.
            if (next[idx] != type) {
                continue;
            }
            
            if (type == CellType::EMPTY || type == CellType::STONE) {
                continue; // These don't move
            }

            if (type == CellType::SAND) {
                if (Point below = pos + DOWN; below.y < _height) {
                    // TODO: Fix, Check the `next` grid to see if the spot is available.
                    // Sand is denser than water, so it will sink.
                    if (const CellType type_below = next[flatten_coords(below)]; type_below == CellType::EMPTY || type_below == CellType::WATER) {
                        next[flatten_coords(below)] = CellType::SAND;
                        next[idx] = type_below; // The water/empty cell moves up
                        continue;
                    }
                }

                // Check diagonals, also allowing sand to sink in water.
                // Randomize check to prevent bias.
                const bool check_left_first = GetRandomValue(0, 1) == 0;

                const Point d_left = pos + DOWN + LEFT;
                const Point d_right = pos + DOWN + RIGHT;
                auto check_diag = [&](const Point diag_pos) {
                    if (diag_pos.y < _height && diag_pos.x >= 0 && diag_pos.x < _width) {
                        if (const CellType type_diag = next[flatten_coords(diag_pos)];type_diag == CellType::EMPTY || type_diag == CellType::WATER) {
                            next[flatten_coords(diag_pos)] = CellType::SAND;
                            next[idx] = type_diag;
                            return true;
                        }
                    }
                    return false;
                };

                if (check_left_first) {
                    if (check_diag(d_left) || check_diag(d_right)) continue;
                } else {
                    if (check_diag(d_right) || check_diag(d_left)) continue;
                }
            }
            else if (type == CellType::WATER)
            {
                if (Point below = pos + DOWN; below.y < _height && next[flatten_coords(below)] == CellType::EMPTY) {
                    next[flatten_coords(below)] = CellType::WATER;
                    next[idx] = CellType::EMPTY;
                    continue;
                }

                const bool check_left_first = GetRandomValue(0, 1) == 0;
                const Point d_left = pos + DOWN + LEFT;
                const Point d_right = pos + DOWN + RIGHT;
                const Point s_left = pos + LEFT;
                const Point s_right = pos + RIGHT;

                auto check_diag = [&](const Point diag_pos, const Point side_pos) {
                    if (
                        diag_pos.y < _height && diag_pos.x >= 0 && diag_pos.x < _width &&
                        next[flatten_coords(diag_pos)] == CellType::EMPTY &&
                        side_pos.x >= 0 && side_pos.x < _width && next[flatten_coords(side_pos)] == CellType::EMPTY
                    ) {
                        next[flatten_coords(diag_pos)] = CellType::WATER;
                        next[idx] = CellType::EMPTY;
                        return true;
                    }
                    return false;
                };

                if (check_left_first) {
                    if (check_diag(d_left, s_left) || check_diag(d_right, s_right)) continue;
                } else {
                    if (check_diag(d_right, s_right) || check_diag(d_left, s_left)) continue;
                }

                auto check_side = [&](const Point side_pos) {
                    if (side_pos.x >= 0 && side_pos.x < _width && next[flatten_coords(side_pos)] == CellType::EMPTY) {
                        next[flatten_coords(side_pos)] = CellType::WATER;
                        next[idx] = CellType::EMPTY;
                        return true;
                    }
                    return false;
                };
                
                if (check_left_first) {
                    if (check_side(s_left) || check_side(s_right)) continue;
                } else {
                    if (check_side(s_right) || check_side(s_left)) continue;
                }
            }
        }
    }

    // Commit the new state for the next frame.
    _cells = std::move(next);
}

void Simulation::set_type_at(const int x, const int y, const CellType type)
{
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        _cells[flatten_coords(x, y)] = type;
    }
}

CellType Simulation::get_type_at(const int x, const int y) const
{
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        return _cells[flatten_coords(x, y)];
    }
    return CellType::EMPTY;
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
