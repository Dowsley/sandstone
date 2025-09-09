//
// Created by João Dowsley on 09/09/25.
//

#ifndef MOVEMENT_UTILS_H
#define MOVEMENT_UTILS_H

#include "../core/cell_matrix.h"
#include "../core/cell_data.h"
#include "../elements/element_type.h"

class MovementUtils {
public:
    // Basic movement operation: move cell from (src_x, src_y) to (dest_x, dest_y)
    static bool move_cell(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int src_x, int src_y,
        int dest_x, int dest_y
    );

    // Try to move in a direction with optional distance
    static bool try_move(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y,
        int dx, int dy,
        int distance = 1
    );

    // Check if a horizontal path is clear for sliding movements
    static bool is_horizontal_path_clear(
        const CellMatrix &next_cells,
        int start_x, int y,
        int direction, // -1 for left, 1 for right
        int distance
    );

    // Check if destination is suitable for movement (empty or specific type)
    static bool can_move_to(
        const CellMatrix &next_cells,
        int x, int y,
        bool allow_liquids = false
    );

    // Try sliding movement in both directions up to max_distance
    static bool try_slide_movement(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y,
        int dy, // vertical offset: -1 for up, 0 for same level, 1 for down
        int max_distance,
        const int dirs[2] // direction preference array
    );

    // Try diagonal movement for solids (can fall through liquids)
    static bool try_solid_diagonal_movement(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y,
        const int dirs[2] // direction preference array
    );
};

#endif //MOVEMENT_UTILS_H
