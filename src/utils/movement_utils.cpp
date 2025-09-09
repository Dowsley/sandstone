//
// Created by João Dowsley on 09/09/25.
//

#include "movement_utils.h"
#include "element_type_checker.h"

bool MovementUtils::move_cell(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int src_x, const int src_y,
    const int dest_x, const int dest_y)
{
    if (!curr_cells.within_bounds(dest_x, dest_y)) {
        return false;
    }

    const ElementType* dest_type = next_cells.get_type(dest_x, dest_y);
    next_cells.get(dest_x, dest_y) = curr_cells.get(src_x, src_y);
    next_cells.get(src_x, src_y) = { dest_type, 0, 0, 0 };
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
    
    if (!curr_cells.within_bounds(dest_x, dest_y)) {
        return false;
    }

    if (can_move_to(next_cells, dest_x, dest_y)) {
        return move_cell(curr_cells, next_cells, x, y, dest_x, dest_y);
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
    const auto pos = Vector2(x, y);
    if (!next_cells.within_bounds(pos)) {
        return false;
    }

    if (next_cells.is_empty(pos)) {
        return true;
    }
    
    if (allow_liquids && next_cells.is_liquid(pos)) {
        return true;
    }
    
    return false;
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
            
            // Check if the destination is available
            if (can_move_to(next_cells, nx, ny)) {
                return move_cell(curr_cells, next_cells, x, y, nx, ny);
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
        if (curr_cells.within_bounds(nx, ny) && curr_cells.within_bounds(nx, y)) {
            const ElementType* diag_type = next_cells.get_type(nx, ny);
            const ElementType* side_type = next_cells.get_type(nx, y);
            if (
                diag_type && side_type &&
                can_move_to(next_cells, nx, ny, true) &&
                can_move_to(next_cells, nx, y, true)
            ) {
                return move_cell(curr_cells, next_cells, x, y, nx, ny);
            }
        }
    }
    return false;
}
