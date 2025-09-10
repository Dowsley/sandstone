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
    // Generation-stamped write mask
    std::vector<uint8_t> _written_gen;
    uint8_t _gen = 1;
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

    bool is_empty(int x, int y) const;
    bool is_of_kind(int x, int y, ElementKind kind) const;
    bool is_of_kinds(int x, int y, std::initializer_list<ElementKind> kinds) const;

    bool is_empty(const Vector2I &pos) const;
    bool is_of_kind(const Vector2I &pos, ElementKind kind) const;
    bool is_of_kinds(const Vector2I &pos, std::initializer_list<ElementKind> kinds) const;


    bool within_bounds(int x, int y) const;
    bool within_bounds(const Vector2I& pos) const;

    // Write-mask API (generation-stamped; increment per tick)
    void begin_tick();
    bool is_written(int x, int y) const;
    void mark_written(int x, int y);
};



#endif //CELL_MATRIX_H
