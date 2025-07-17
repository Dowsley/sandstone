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
};

#endif //CELL_DATA_H
