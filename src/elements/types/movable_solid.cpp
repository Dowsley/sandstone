//
// Created by João Dowsley on 15/07/25.
//

#include "movable_solid.h"
#include "../../core/cell_matrix.h"
#include "../../utils/element_type_checker.h"
#include "../../utils/movement_utils.h"

#include <random>
#include "../../utils/random_utils.h"

bool MovableSolid::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const
{
    // TODO: This is not sinking over gas. Fix it. Make density feature while Im at it.
    
    // Try to move down (can fall through liquids)
    if (MovementUtils::can_move_to(next_cells, x, y + 1, true)) {
        return MovementUtils::move_cell(curr_cells, next_cells, x, y, x, y + 1);
    }

    // Try diagonals, but only if the side cell is also empty or water (no squeezing)
    int dirs[2] = {-1, 1};
    if (RandomUtils::coin_flip()) std::swap(dirs[0], dirs[1]);

    if (MovementUtils::try_solid_diagonal_movement(curr_cells, next_cells, x, y, dirs)) {
        return true;
    }

    // No movement
    return false;
}