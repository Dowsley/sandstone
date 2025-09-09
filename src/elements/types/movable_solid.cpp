//
// Created by João Dowsley on 15/07/25.
//

#include "movable_solid.h"
#include "../../core/cell_matrix.h"
#include "../../utils/element_type_checker.h"
#include "../../utils/movement_utils.h"

#include <random>

bool MovableSolid::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const
{
    // Try to move down (can fall through liquids)
    if (MovementUtils::can_move_to(next_cells, x, y + 1, true)) {
        return MovementUtils::move_cell(curr_cells, next_cells, x, y, x, y + 1);
    }

    // TODO: Better randomize this with a random util lib
    // Try diagonals, but only if the side cell is also empty or water (no squeezing)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    int dirs[2] = {-1, 1};
    if (dist(gen)) std::swap(dirs[0], dirs[1]);

    if (MovementUtils::try_solid_diagonal_movement(curr_cells, next_cells, x, y, dirs)) {
        return true;
    }

    // No movement
    return false;
}