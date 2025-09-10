//
// Created by João Dowsley on 09/09/25.
//

#include "movement_utils.h"
#include "element_type_checker.h"
#include "../elements/element_type.h"
#include "../core/cell_matrix.h"
#include "../core/cell_data.h"
#include "../utils/random_utils.h"

bool MovementUtils::move_cell(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int src_x, const int src_y,
    const int dest_x, const int dest_y)
{
    if (!curr_cells.within_bounds(dest_x, dest_y)) {
        return false;
    }

    if (next_cells.is_written(dest_x, dest_y)) {
        return false;
    }

    const ElementType* dest_type = next_cells.get_type(dest_x, dest_y);
    next_cells.get(dest_x, dest_y) = curr_cells.get(src_x, src_y);
    next_cells.get(src_x, src_y) = { dest_type, 0, 0, 0 };
    next_cells.mark_written(dest_x, dest_y);
    return true;
}

static int density_of(const ElementType* t)
{
    return t ? t->get_density() : 0;
}

bool MovementUtils::can_displace(
    const CellMatrix &curr_cells,
    const CellMatrix &next_cells,
    const int x, const int y,
    const int nx, const int ny)
{
    if (!curr_cells.within_bounds(nx, ny)) return false;

    const ElementType* src = next_cells.get_type(x, y);
    const ElementType* dst = next_cells.get_type(nx, ny);
    if (!src || !dst) return false;

    // Immovable solid at destination: never displace
    if (ElementTypeChecker::is_of_kind(*dst, ElementKind::ImmovableSolid))
        return false;

    // Source immovable: it shouldn't be trying to move anyway
    if (ElementTypeChecker::is_of_kind(*src, ElementKind::ImmovableSolid))
        return false;

    const int dy = ny - y;
    const int s = density_of(src);
    const int d = density_of(dst);

    if (dy > 0) { // moving down
        return s > d;
    }
    if (dy < 0) { // moving up
        return s < d;
    }
    
    // lateral
    // Allow moving into EMPTY, or displace when source is denser than destination
    if (ElementTypeChecker::is_empty(*dst)) return true;
    return s > d;
}

bool MovementUtils::swap_or_move(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y,
    const int nx, const int ny)
{
    if (!curr_cells.within_bounds(nx, ny)) return false;
    if (next_cells.is_written(nx, ny)) return false;

    const int dest_idx_taken = next_cells.flatten_coords(nx, ny);
    (void)dest_idx_taken;

    const ElementType* dst_type = next_cells.get_type(nx, ny);

    if (ElementTypeChecker::is_empty(*dst_type)) {
        // Simple move
        return move_cell(curr_cells, next_cells, x, y, nx, ny);
    }

    // Swap with destination
    const CellData src_cell = curr_cells.get(x, y);
    const CellData dst_cell = curr_cells.get(nx, ny);

    next_cells.get(nx, ny) = src_cell;
    next_cells.get(x, y) = dst_cell;

    next_cells.mark_written(nx, ny);
    next_cells.mark_written(x, y);
    return true;
}

bool MovementUtils::try_move(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y,
    const int dx, const int dy,
    const int distance)
{
    const int dest_x = x + dx * distance;
    const int dest_y = y + dy * distance;
    if (!curr_cells.within_bounds(dest_x, dest_y)) return false;
    if (!can_displace(curr_cells, next_cells, x, y, dest_x, dest_y)) return false;
    return swap_or_move(curr_cells, next_cells, x, y, dest_x, dest_y);
}

bool MovementUtils::try_lateral_wiggle(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y,
    const int dirs[2])
{
    // Gated randomness to reduce ping-pong meshes
    if (!RandomUtils::coin_flip()) {
        return false;
    }
    for (int i = 0; i < 2; ++i) {
        const int nx = x + dirs[i];
        const int ny = y;
        if (!curr_cells.within_bounds(nx, ny)) continue;
        if (can_displace(curr_cells, next_cells, x, y, nx, ny)) {
            return swap_or_move(curr_cells, next_cells, x, y, nx, ny);
        }
    }
    return false;
}

bool MovementUtils::is_horizontal_path_clear(
    const CellMatrix &next_cells,
    const int start_x, const int y,
    const int direction,
    const int distance)
{
    for (int j = 1; j <= distance; ++j) {
        const int check_x = start_x + direction * j;
        if (!next_cells.within_bounds(check_x, y)) {
            return false;
        }
        const ElementType* type = next_cells.get_type(check_x, y);
        if (!ElementTypeChecker::is_empty(*type)) {
            return false;
        }
    }
    return true;
}

bool MovementUtils::can_move_to(
    const CellMatrix &next_cells,
    const int x, const int y,
    const bool allow_liquids)
{
    const auto pos = Vector2I(x, y);
    if (!next_cells.within_bounds(pos)) return false;
    if (next_cells.is_written(x, y)) return false;
    return next_cells.is_empty(pos);
}

bool MovementUtils::try_slide_movement(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y,
    const int dy,
    const int max_distance,
    const int dirs[2])
{
    for (int i = 1; i <= max_distance; ++i) {
        for (int d = 0; d < 2; ++d) {
            const int dir = dirs[d];
            const int nx = x + dir * i;
            const int ny = y + dy;
            
            if (!curr_cells.within_bounds(nx, ny)) continue;
            
            // Check if the horizontal path is clear
            if (!is_horizontal_path_clear(next_cells, x, y, dir, i)) {
                continue;
            }
            
            // Density-based displacement
            if (can_displace(curr_cells, next_cells, x, y, nx, ny)) {
                return swap_or_move(curr_cells, next_cells, x, y, nx, ny);
            }
        }
    }
    return false;
}

bool MovementUtils::try_solid_diagonal_movement(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y,
    const int dirs[2])
{
    for (int i = 0; i < 2; ++i) {
        const int nx = x + dirs[i];
        const int ny = y + 1;
        if (!curr_cells.within_bounds(nx, ny)) continue;
        if (!is_horizontal_path_clear(next_cells, x, y, dirs[i], 1)) continue;
        if (can_displace(curr_cells, next_cells, x, y, nx, ny)) {
            return swap_or_move(curr_cells, next_cells, x, y, nx, ny);
        }
    }
    return false;
}
