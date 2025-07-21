//
// Created by João Dowsley on 15/07/25.
//

#include "liquid.h"
#include "../../core/cell_matrix.h"
#include <cstdlib>

bool Liquid::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const 
{
    if (next_cells.get_type(x, y) != curr_cells.get_type(x, y)) {
        return false;
    }

    constexpr int max_slide = 3;
    int dirs[2] = {-1, 1};
    // Randomize direction order
    if (rand() % 2)
        std::swap(dirs[0], dirs[1]);

    // 1. Try to move down
    const int down_y = y + 1;
    if (curr_cells.within_bounds(x, down_y)) {
        const ElementType* dest_type = next_cells.get_type(x, down_y);
        if (dest_type->get_id() == "EMPTY") {
            next_cells.get(x, down_y) = curr_cells.get(x, y);
            next_cells.get(x, y) = { dest_type, 0, 0, 0 };
            return true;
        }
    }

    // 2. Try to move diagonally down (slide up to max_slide)
    for (int d = 0; d < 2; ++d) {
        const int dir = dirs[d];
        for (int i = 1; i <= max_slide; ++i) {
            const int nx = x + dir * i;
            const int ny = y + 1;
            if (!curr_cells.within_bounds(nx, ny)) break;
            // Path must be clear horizontally
            bool path_clear = true;
            for (int j = 1; j <= i; ++j) {
                if (next_cells.get_type(x + dir * j, y)->get_id() != "EMPTY") {
                    path_clear = false;
                    break;
                }
            }
            if (!path_clear)
                break;
            const ElementType* dest_type = next_cells.get_type(nx, ny);
            if (dest_type->get_id() == "EMPTY") {
                next_cells.get(nx, ny) = curr_cells.get(x, y);
                next_cells.get(x, y) = { dest_type, 0, 0, 0 };
                return true;
            }
        }
    }

    // 3. Try to move sideways (slide up to max_slide)
    for (int d = 0; d < 2; ++d) {
        const int dir = dirs[d];
        for (int i = 1; i <= max_slide; ++i) {
            const int nx = x + dir * i;
            if (!curr_cells.within_bounds(nx, y)) break;
            // Path must be clear horizontally
            bool path_clear = true;
            for (int j = 1; j <= i; ++j) {
                if (next_cells.get_type(x + dir * j, y)->get_id() != "EMPTY") {
                    path_clear = false;
                    break;
                }
            }
            if (!path_clear) break;
            const ElementType* dest_type = next_cells.get_type(nx, y);
            if (dest_type->get_id() == "EMPTY") {
                next_cells.get(nx, y) = curr_cells.get(x, y);
                next_cells.get(x, y) = { dest_type, 0, 0, 0 };
                return true;
            }
        }
    }

    // 4. No move was possible
    next_cells.get(x, y) = curr_cells.get(x, y);
    return false;
}