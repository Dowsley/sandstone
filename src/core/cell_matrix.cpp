//
// Created by João Dowsley on 16/07/25.
//

#include "cell_matrix.h"

CellMatrix::CellMatrix(const int width, const int height, const ElementRegistry &element_registry)
    : _width(width), _height(height)
{
    _cells.resize(width * height,
        CellData { element_registry.get_type_by_id("EMPTY"), 0 });
}

int CellMatrix::flatten_coords(const int x, const int y) const
{
    return y * _width + x;
}

int CellMatrix::get_width() const { return _width; }
int CellMatrix::get_height() const { return _height; }

CellData& CellMatrix::get(const int x, const int y)
{
    const int idx = flatten_coords(x, y);
    return get(idx);
}

CellData & CellMatrix::get(const int idx)
{
    return _cells[idx];
}

const ElementType* CellMatrix::get_type(const int x, const int y) const
{
    const int idx = flatten_coords(x, y);
    return get_type(idx);
}

const ElementType* CellMatrix::get_type(const int idx) const
{
    return _cells[idx].type;
}

int CellMatrix::get_color_variation_index(const int x, const int y) const
{
    const int idx = flatten_coords(x, y);
    return get_color_variation_index(idx);
}

int CellMatrix::get_color_variation_index(const int idx) const
{
    return _cells[idx].color_variant_index;
}

void CellMatrix::set(const int x, const int y, const CellData &cell_data)
{
    const int idx = flatten_coords(x, y);
    _cells[idx] = cell_data;
}

void CellMatrix::set_type(const int x, const int y, const ElementType *type)
{
    const int idx = flatten_coords(x, y);
    _cells[idx].type = type;
}

void CellMatrix::set_color_variation_index(const int x, const int y, uint8_t color_variant_index)
{
    const int idx = flatten_coords(x, y);
    _cells[idx].color_variant_index = color_variant_index;
}
