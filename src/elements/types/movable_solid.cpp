//
// Created by João Dowsley on 15/07/25.
//

#include "movable_solid.h"
#include "../../core/cell_matrix.h"
#include <random>

bool MovableSolid::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const
{
    // Try to move down
    if (curr_cells.within_bounds(x, y + 1)) {
        // TODO: make this NOT hardcoded for types id, but instead for types.
        // TODO: In fact, might as well just fix all of this system lol system
        const auto below_type = next_cells.get_type(x, y + 1);
        if (below_type &&
            (below_type->get_id() == "EMPTY"
                || below_type->get_id() == "WATER"
                || below_type->get_id() == "STEAM"
            )
        ) {
            next_cells.get(x, y + 1) = curr_cells.get(x, y);
            next_cells.get(x, y) = CellData{below_type, 0};
            return true;
        }
    }

    // Try diagonals, but only if the side cell is also empty or water (no squeezing)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution dist(0, 1);
    int dirs[2] = {-1, 1};
    if (dist(gen)) std::swap(dirs[0], dirs[1]);

    for (int i = 0; i < 2; ++i) {
        const int nx = x + dirs[i];
        const int ny = y + 1;
        if (curr_cells.within_bounds(nx, ny) && curr_cells.within_bounds(nx, y)) {
            const ElementType* diag_type = next_cells.get_type(nx, ny);
            const ElementType* side_type = next_cells.get_type(nx, y);
            if (
                diag_type && side_type &&
                (diag_type->get_id() == "EMPTY" || diag_type->get_id() == "WATER") &&
                (side_type->get_id() == "EMPTY" || side_type->get_id() == "WATER")
            ) {
                next_cells.get(nx, ny) = curr_cells.get(x, y);
                next_cells.get(x, y) = CellData{diag_type, 0};
                return true;
            }
        }
    }

    // No movement
    return false;
}