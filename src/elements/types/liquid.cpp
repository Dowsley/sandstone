//
// Created by João Dowsley on 15/07/25.
//

#include "liquid.h"
#include "../../core/cell_matrix.h"
#include "../../utils/element_type_checker.h"
#include "../../utils/movement_utils.h"
#include "../../utils/random_utils.h"

bool Liquid::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const 
{
    if (next_cells.is_written(x, y))
        return false;

    constexpr int max_slide = 3;
    
    // Randomize direction choice
    int dirs[2];
    if (RandomUtils::coin_flip()) { dirs[0] = -1; dirs[1] = 1; }
    else { dirs[0] = 1; dirs[1] = -1; }

    // 1. Try to move down
    if (MovementUtils::try_move(curr_cells, next_cells, x, y, 0, 1)) {
        return true;
    }

    // 2. Try to move diagonally down (slide up to max_slide)
    if (MovementUtils::try_slide_movement(curr_cells, next_cells, x, y, 1, max_slide, dirs)) {
        return true;
    }

    // 3. Try to move sideways (slide up to max_slide)
    if (MovementUtils::try_slide_movement(curr_cells, next_cells, x, y, 0, max_slide, dirs)) {
        return true;
    }

    // 4. No move was possible
    next_cells.get(x, y) = curr_cells.get(x, y);
    return false;
}