//
// Created by João Dowsley on 08/08/25.
//

#include "gas.h"

#include "../../core/cell_matrix.h"
#include <cstdlib>

bool Gas::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const 
{
    // TODO: Consolidate shared logic between gas and liquid
    if (next_cells.get_type(x, y) != curr_cells.get_type(x, y)) {
        return false;
    }

    constexpr int max_dispersion = 4;
    
    // Gas behavior: more chaotic movement patterns
    // Randomize movement preference - gases are more unpredictable
    const int movement_choice = rand() % 100;
    
    // Truly randomize direction choice
    int dirs[2];
    if (rand() % 2) {
        dirs[0] = -1; dirs[1] = 1;  // Left first
    } else {
        dirs[0] = 1; dirs[1] = -1;  // Right first
    }

    // 1. Try to move up (gases rise) - 60% preference
    if (movement_choice < 60) {
        const int up_y = y - 1;
        if (curr_cells.within_bounds(x, up_y)) {
            const ElementType* dest_type = next_cells.get_type(x, up_y);
            if (dest_type->get_id() == "EMPTY") {
                next_cells.get(x, up_y) = curr_cells.get(x, y);
                next_cells.get(x, y) = { dest_type, 0, 0, 0 };
                return true;
            }
        }
    }

    // 2. Try to move diagonally up (gases spread as they rise)
    for (int i = 1; i <= max_dispersion; ++i) {
        for (int d = 0; d < 2; ++d) {
            const int dir = dirs[d];
            const int nx = x + dir * i;
            const int ny = y - 1;
            if (!curr_cells.within_bounds(nx, ny)) continue;
            
            // Path must be clear horizontally
            bool path_clear = true;
            for (int j = 1; j <= i; ++j) {
                if (next_cells.get_type(x + dir * j, y)->get_id() != "EMPTY") {
                    path_clear = false;
                    break;
                }
            }
            if (!path_clear) continue;
            
            const ElementType* dest_type = next_cells.get_type(nx, ny);
            if (dest_type->get_id() == "EMPTY") {
                next_cells.get(nx, ny) = curr_cells.get(x, y);
                next_cells.get(x, y) = { dest_type, 0, 0, 0 };
                return true;
            }
        }
    }

    // 3. Try to move sideways (horizontal dispersion) - gases spread out more aggressively
    for (int i = 1; i <= max_dispersion; ++i) {
        for (int d = 0; d < 2; ++d) {
            const int dir = dirs[d];
            const int nx = x + dir * i;
            if (!curr_cells.within_bounds(nx, y)) continue;
            
            // Path must be clear horizontally
            bool path_clear = true;
            for (int j = 1; j <= i; ++j) {
                if (next_cells.get_type(x + dir * j, y)->get_id() != "EMPTY") {
                    path_clear = false;
                    break;
                }
            }
            if (!path_clear) continue;
            
            const ElementType* dest_type = next_cells.get_type(nx, y);
            if (dest_type->get_id() == "EMPTY") {
                next_cells.get(nx, y) = curr_cells.get(x, y);
                next_cells.get(x, y) = { dest_type, 0, 0, 0 };
                return true;
            }
        }
    }

    // 4. Occasionally move down (settling behavior when blocked) - 10% chance
    if (movement_choice >= 90) {
        const int down_y = y + 1;
        if (curr_cells.within_bounds(x, down_y)) {
            const ElementType* dest_type = next_cells.get_type(x, down_y);
            if (dest_type->get_id() == "EMPTY") {
                next_cells.get(x, down_y) = curr_cells.get(x, y);
                next_cells.get(x, y) = { dest_type, 0, 0, 0 };
                return true;
            }
        }
    }

    // 5. No move was possible - stay in place
    next_cells.get(x, y) = curr_cells.get(x, y);
    return false;
}