//
// Created by João Dowsley on 16/07/25.
//

#include "cell_matrix.h"
#include "../utils/element_type_checker.h"

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

bool CellMatrix::is_of_type(const int x, const int y,
    const std::string &type_id) const
{
    const auto type = get_type(x, y);
    if (type == nullptr)
        return false;
    return type->get_id() == type_id;
}

bool CellMatrix::is_of_type(const Vector2I &pos, const std::string &type_id) const
{
    return is_of_type(pos.x, pos.y, type_id);
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

bool CellMatrix::is_empty(const int x, const int y) const
{
    const ElementType *dest_type = get_type(x, y);
    return ElementTypeChecker::is_empty(*dest_type);
}

bool CellMatrix::is_liquid(const int x, const int y) const
{
    const ElementType *dest_type = get_type(x, y);
    return ElementTypeChecker::is_liquid(*dest_type);
}

bool CellMatrix::is_immovable_solid(const int x, const int y) const
{
    const ElementType *dest_type = get_type(x, y);
    return ElementTypeChecker::is_immovable_solid(*dest_type);
}

bool CellMatrix::is_movable_solid(const int x, const int y) const
{
    const ElementType *dest_type = get_type(x, y);
    return ElementTypeChecker::is_movable_solid(*dest_type);
}

bool CellMatrix::is_solid(const int x, const int y) const
{
    const ElementType *dest_type = get_type(x, y);
    return ElementTypeChecker::is_solid(*dest_type);
}

bool CellMatrix::is_gas(const int x, const int y) const
{
    const ElementType *dest_type = get_type(x, y);
    return ElementTypeChecker::is_gas(*dest_type);
}

bool CellMatrix::is_empty(const Vector2 &pos) const
{
    return is_empty(pos.x, pos.y);
}

bool CellMatrix::is_liquid(const Vector2 &pos) const
{
    return is_liquid(pos.x, pos.y);
}

bool CellMatrix::is_immovable_solid(const Vector2 &pos) const
{
    return is_immovable_solid(pos.x, pos.y);
}

bool CellMatrix::is_movable_solid(const Vector2 &pos) const
{
    return is_movable_solid(pos.x, pos.y);
}

bool CellMatrix::is_solid(const Vector2 &pos) const
{
    return is_solid(pos.x, pos.y);
}

bool CellMatrix::is_gas(const Vector2 &pos) const
{
    return is_gas(pos.x, pos.y);
}

bool CellMatrix::within_bounds(const int x, const int y) const
{
    return x >= 0 && x < _width && y >= 0 && y < _height;
}

bool CellMatrix::within_bounds(const Vector2 &pos) const
{
    return within_bounds(pos.x, pos.y);
}
