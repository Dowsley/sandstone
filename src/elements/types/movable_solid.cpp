//
// Created by João Dowsley on 15/07/25.
//

#include "movable_solid.h"
#include "../../utils/element_type_checker.h"
#include "../../utils/movement_utils.h"

#include <random>
#include "../../utils/random_utils.h"

MovableSolid::MovableSolid()
{
    _kind = ElementKind::MovableSolid;
}

bool MovableSolid::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const
{
    // Density-based: try to move down (displace or swap if heavier)
    if (MovementUtils::try_move(curr_cells, next_cells, x, y, 0, 1)) {
        return true;
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