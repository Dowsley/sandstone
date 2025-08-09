//
// Created by João Dowsley on 16/07/25.
//

#ifndef CELL_MATRIX_H
#define CELL_MATRIX_H

#include "cell_data.h"
#include "../elements/element_registry.h"
#include "../types/vector2i.h"

#include <vector>

class CellMatrix {
private:
    std::vector<CellData> _cells;
    int _width, _height;
public:
    CellMatrix() : _width(0), _height(0) {}
    CellMatrix(int width, int height, const ElementRegistry &element_registry);

    int flatten_coords(int x, int y) const;

    int get_width() const;
    int get_height() const;
    CellData& get(int x, int y);
    CellData& get(int idx);
    const ElementType* get_type(int x, int y) const;
    const ElementType* get_type(int idx) const;
    bool is_of_type(int x, int y, const std::string &type_id) const;
    bool is_of_type(const Vector2I &pos, const std::string &type_id) const;
    int get_color_variation_index(int x, int y) const;
    int get_color_variation_index(int idx) const;
    void set(int x, int y, const CellData &cell_data);
    void set_type(int x, int y, const ElementType *type);
    void set_color_variation_index(int x, int y, uint8_t color_variant_index);

    bool within_bounds(int x, int y) const { return x >= 0 && x < _width && y >= 0 && y < _height; }
};



#endif //CELL_MATRIX_H
