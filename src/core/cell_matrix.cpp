//
// Created by João Dowsley on 16/07/25.
//

#include "cell_matrix.h"
#include "../utils/element_type_checker.h"
#include "../types/vector2i.h"

CellMatrix::CellMatrix(const int width, const int height, const ElementRegistry &element_registry)
    : _width(width), _height(height)
{
    _cells.resize(width * height,
        CellData { element_registry.get_type_by_id("EMPTY"), 0 });
    _written_gen.assign(width * height, 0);
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

void CellMatrix::set_color_variation_index(const int x, const int y, const uint8_t color_variant_index)
{
    const int idx = flatten_coords(x, y);
    _cells[idx].color_variant_index = color_variant_index;
}

bool CellMatrix::is_empty(const int x, const int y) const
{
    const ElementType *dest_type = get_type(x, y);
    return ElementTypeChecker::is_empty(*dest_type);
}

bool CellMatrix::is_of_kind(const int x, const int y, const ElementKind kind) const
{
    const ElementType *dest_type = get_type(x, y);
    return ElementTypeChecker::is_of_kind(*dest_type, kind);
}

bool CellMatrix::is_of_kinds(const int x, const int y, const std::initializer_list<ElementKind> kinds) const
{
    const ElementType *dest_type = get_type(x, y);
    return ElementTypeChecker::is_of_kinds(*dest_type, kinds);
}

bool CellMatrix::is_empty(const Vector2I &pos) const
{
    return is_empty(pos.x, pos.y);
}

bool CellMatrix::is_of_kind(const Vector2I &pos, const ElementKind kind) const
{
    return is_of_kind(pos.x, pos.y, kind);
}

bool CellMatrix::is_of_kinds(const Vector2I &pos, const std::initializer_list<ElementKind> kinds) const
{
    return is_of_kinds(pos.x, pos.y, kinds);
}

bool CellMatrix::within_bounds(const int x, const int y) const
{
    return x >= 0 && x < _width && y >= 0 && y < _height;
}

bool CellMatrix::within_bounds(const Vector2I &pos) const
{
    return within_bounds(pos.x, pos.y);
}

void CellMatrix::begin_tick()
{
    // Increment generation; on wrap, reset to 1 and clear buffer
    const uint8_t next = static_cast<uint8_t>(_gen + 1);
    if (next == 0) {
        _gen = 1;
        std::ranges::fill(_written_gen, 0);
    } else {
        _gen = next;
    }
}

bool CellMatrix::is_written(const int x, const int y) const
{
    const int idx = flatten_coords(x, y);
    return _written_gen[idx] == _gen;
}

void CellMatrix::mark_written(const int x, const int y)
{
    const int idx = flatten_coords(x, y);
    _written_gen[idx] = _gen;
}
