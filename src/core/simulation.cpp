#include "simulation.h"
#include <utility> // For std::move

Simulation::Simulation(const int width, const int height, ElementRegistry& element_registry)
    : _element_registry(element_registry)
{
    _width = width;
    _height = height;
    _element_registry.initialize();
    _cells = CellMatrix(width, height, _element_registry);
    _next_cells = CellMatrix(width, height, _element_registry);
}

Simulation::Simulation(const Point &size, ElementRegistry& element_registry)
    : Simulation(size.x, size.y, element_registry) { }

void Simulation::step()
{
    _next_cells = _cells;
    
    for (int y = _height - 1; y >= 0; --y) {
        for (int x = 0; x < _width; ++x) {
            if (const ElementType *type = _cells.get_type(x, y)) {
                type->step_particle_at(_cells, _next_cells, x, y, type);
            }
        }
    }

    _cells = std::move(_next_cells);
}

void Simulation::set_type_at(const int x, const int y, const ElementType *type, const int colorIdx)
{
    if (x >= 0 && x < _width && y >= 0 && y < _height) { // TODO: Encapsulate this under a `within_bounds` func
        _cells.get(x, y).type = type;
        if (colorIdx > -1)
            _cells.set_color_variation_index(x, y, colorIdx);
    }
}

void Simulation::set_type_at(const int x, const int y, const std::string &id, const int colorIdx)
{
    set_type_at(x, y, _element_registry.get_type_by_id(id), colorIdx);
}

const ElementType* Simulation::get_type_at(const int x, const int y) const
{
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        return _cells.get_type(x, y);
    }
    return nullptr;
}

void Simulation::fill_render_buffer(Color *dst) const
{
    const int total = _width * _height;
    for (int i = 0; i < total; ++i) {
        if (const auto type = _cells.get_type(i)) {
            dst[i] = type->get_color(_cells.get_color_variation_index(i));
        } else {
            dst[i] = { 0, 0, 0, 0 }; // Black for null types
        }
    }
}

int Simulation::get_width() const { return _width; }
int Simulation::get_height() const { return _height; }

int Simulation::flatten_coords(const int x, const int y) const
{
    return y * _width + x;
}

int Simulation::flatten_coords(const Point &pos) const
{
    return flatten_coords(pos.x, pos.y);
}

ElementType* Simulation::get_type_by_id(const std::string &id) const
{
    return _element_registry.get_type_by_id(id);
}

std::vector<const ElementType *> Simulation::get_all_element_types() const
{
    return _element_registry.get_all_types();
}
