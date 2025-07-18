//
// Created by João Dowsley on 15/07/25.
//

#include "liquid.h"

#include "../../core/cell_matrix.h"
#include <random>

// Global/static bias for water direction, alternates each step
static int global_water_bias = 1;

bool Liquid::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const
{
    // Only move if this cell hasn't already been moved this step
    if (next_cells.get_type(x, y) != curr_cells.get_type(x, y)) {
        return false;
    }

    int width = curr_cells.get_width();
    int height = curr_cells.get_height();

    // Try to move down
    if (curr_cells.within_bounds(x, y + 1)) {
        const ElementType* below_type = next_cells.get_type(x, y + 1);
        if (below_type && below_type->get_id() == "EMPTY") {
            next_cells.get(x, y + 1) = curr_cells.get(x, y);
            next_cells.get(x, y) = CellData{below_type, 0};
            return true;
        }
    }

    // Use global bias for direction
    const int dirs[2] = {global_water_bias, -global_water_bias};

    // Diagonal checks (only if both diagonal and side are empty)
    for (int i = 0; i < 2; ++i) {
        const int nx = x + dirs[i];
        const int ny = y + 1;
        if (curr_cells.within_bounds(nx, ny) && curr_cells.within_bounds(nx, y)) {
            const ElementType* diag_type = next_cells.get_type(nx, ny);
            const ElementType* side_type = next_cells.get_type(nx, y);
            if (diag_type && side_type &&
                diag_type->get_id() == "EMPTY" &&
                side_type->get_id() == "EMPTY") {
                next_cells.get(nx, ny) = curr_cells.get(x, y);
                next_cells.get(x, y) = CellData{diag_type, 0};
                return true;
            }
        }
    }

    // Horizontal spread
    for (int i = 0; i < 2; ++i) {
        int nx = x + dirs[i];
        if (curr_cells.within_bounds(nx, y)) {
            const ElementType* side_type = next_cells.get_type(nx, y);
            if (side_type && side_type->get_id() == "EMPTY") {
                next_cells.get(nx, y) = curr_cells.get(x, y);
                next_cells.get(x, y) = CellData{side_type, 0};
                return true;
            }
        }
    }

    // No movement
    return false;
}

// Call this at the start of each simulation step (e.g., in Simulation::step)
void liquid_flip_global_bias() {
    global_water_bias *= -1;
}