//
// Created by João Dowsley on 09/09/25.
//

#ifndef MOVEMENT_UTILS_H
#define MOVEMENT_UTILS_H

#include "../elements/element_type.h"

class MovementUtils {
public:
    /**
     * @brief Move a cell from (src_x, src_y) to (dest_x, dest_y) in the next buffer.
     * @param curr_cells Current simulation buffer (read source from here).
     * @param next_cells Next simulation buffer (write destination here).
     * @param src_x Source X coordinate.
     * @param src_y Source Y coordinate.
     * @param dest_x Destination X coordinate.
     * @param dest_y Destination Y coordinate.
     * @return True if the move was performed; false if blocked/out-of-bounds/already written.
     */
    static bool move_cell(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int src_x, int src_y,
        int dest_x, int dest_y
    );

    /**
     * @brief Check density rule to decide if (x, y) can displace/swap with (nx, ny).
     * @details Downward moves require source_density > dest_density; upward moves require
     *          source_density < dest_density; lateral only displaces EMPTY. Immovable solids
     *          cannot be displaced.
     * @param curr_cells Current simulation buffer.
     * @param next_cells Next simulation buffer (used for type/lookups and write-mask checks).
     * @param x Source X coordinate.
     * @param y Source Y coordinate.
     * @param nx Destination X coordinate.
     * @param ny Destination Y coordinate.
     * @return True if the displacement is allowed by the rule and bounds; otherwise false.
     */
    static bool can_displace(
        const CellMatrix &curr_cells,
        const CellMatrix &next_cells,
        int x, int y,
        int nx, int ny
    );

    /**
     * @brief Execute either a move (into EMPTY) or a swap (with non-EMPTY) and mark write mask.
     * @param curr_cells Current simulation buffer.
     * @param next_cells Next simulation buffer (write target).
     * @param x Source X coordinate.
     * @param y Source Y coordinate.
     * @param nx Destination X coordinate.
     * @param ny Destination Y coordinate.
     * @return True if the move/swap succeeded; otherwise false.
     */
    static bool swap_or_move(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y,
        int nx, int ny
    );

    /**
     * @brief Try to move by (dx, dy), optionally with a distance multiplier.
     * @param curr_cells Current simulation buffer.
     * @param next_cells Next simulation buffer.
     * @param x Source X coordinate.
     * @param y Source Y coordinate.
     * @param dx Delta X direction (unit step).
     * @param dy Delta Y direction (unit step).
     * @param distance Distance multiplier (>= 1).
     * @return True if movement happened; otherwise false.
     */
    static bool try_move(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y,
        int dx, int dy,
        int distance = 1
    );

    /**
     * @brief Check if the horizontal path is clear for sliding by a given distance.
     * @param next_cells Next simulation buffer.
     * @param start_x Starting X coordinate.
     * @param y Row to check.
     * @param direction -1 for left, 1 for right.
     * @param distance Number of cells to check along the row.
     * @return True if each intermediate cell is EMPTY; otherwise false.
     */
    static bool is_horizontal_path_clear(
        const CellMatrix &next_cells,
        int start_x, int y,
        int direction,
        int distance
    );

    /**
     * @brief Legacy: only checks EMPTY and write-mask (kept for compatibility).
     * @param next_cells Next simulation buffer.
     * @param x Destination X coordinate.
     * @param y Destination Y coordinate.
     * @param allow_liquids Ignored; retained for signature compatibility.
     * @return True if destination is EMPTY and not already written.
     */
    static bool can_move_to(
        const CellMatrix &next_cells,
        int x, int y,
        bool allow_liquids = false
    );

    /**
     * @brief Try sliding movement up to max_distance in preferred horizontal directions.
     * @param curr_cells Current simulation buffer.
     * @param next_cells Next simulation buffer.
     * @param x Source X coordinate.
     * @param y Source Y coordinate.
     * @param dy Vertical offset (-1 up, 0 same row, 1 down).
     * @param max_distance Maximum lateral distance to slide.
     * @param dirs Two-element array of horizontal direction preference (e.g., {-1, 1}).
     * @return True if a slide occurred; otherwise false.
     */
    static bool try_slide_movement(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y,
        int dy,
        int max_distance,
        const int dirs[2]
    );

    /**
     * @brief Try single-step lateral displacement (wiggle) using density rule.
     * @param curr_cells Current simulation buffer.
     * @param next_cells Next simulation buffer.
     * @param x Source X coordinate.
     * @param y Source Y coordinate.
     * @param dirs Two-element array with a randomized order of {-1, 1}.
     * @return True if a lateral move/swaps happened; otherwise false.
     */
    static bool try_lateral_wiggle(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y,
        const int dirs[2]
    );

    /**
     * @brief Try diagonal movement for solids (respects density rule and path clearance).
     * @param curr_cells Current simulation buffer.
     * @param next_cells Next simulation buffer.
     * @param x Source X coordinate.
     * @param y Source Y coordinate.
     * @param dirs Two-element array of horizontal direction preference (e.g., {-1, 1}).
     * @return True if a diagonal move occurred; otherwise false.
     */
    static bool try_solid_diagonal_movement(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y,
        const int dirs[2]
    );
};

#endif //MOVEMENT_UTILS_H
