//
// Created by João Dowsley on 08/08/25.
//

#include "gas.h"

#include "../../core/cell_matrix.h"
#include "../../utils/element_type_checker.h"
#include "../../utils/movement_utils.h"
#include <cstdlib>

bool Gas::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const 
{
    // Guard: do not process a cell whose output position has been claimed in next
    if (next_cells.is_written(x, y)) return false;

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
        if (MovementUtils::try_move(curr_cells, next_cells, x, y, 0, -1)) {
            return true;
        }
    }

    // 2. Try to move diagonally up (gases spread as they rise)
    if (MovementUtils::try_slide_movement(curr_cells, next_cells, x, y, -1, max_dispersion, dirs)) {
        return true;
    }

    // 3. Try to move sideways (horizontal dispersion) - gases spread out more aggressively
    if (MovementUtils::try_slide_movement(curr_cells, next_cells, x, y, 0, max_dispersion, dirs)) {
        return true;
    }

    // 4. Occasionally move down (settling behavior when blocked) - 10% chance
    if (movement_choice >= 90) {
        if (MovementUtils::try_move(curr_cells, next_cells, x, y, 0, 1)) {
            return true;
        }
    }

    // 5. No move was possible - stay in place
    next_cells.get(x, y) = curr_cells.get(x, y);
    return false;
}