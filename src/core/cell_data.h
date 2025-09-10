//
// Created by João Dowsley on 15/07/25.
//

#ifndef CELL_DATA_H
#define CELL_DATA_H

#include <cstdint>

class ElementType;

struct CellData {
    const ElementType *type; // We can change this to point elsewhere, but not change the instance itself.
    uint8_t color_variant_index = 0;
    int8_t vel_x = 0; // -1 for left, 1 for right, 0 for none
    int8_t vel_y = 0; // -1 for up, 1 for down, 0 for none
    int temp_c = 25;
};

#endif //CELL_DATA_H
