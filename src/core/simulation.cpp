#include "simulation.h"
#include <utility> // For std::move

Simulation::Simulation(const int width, const int height, const ElementRegistry &elementRegistry)
    : _elementRegistry(elementRegistry), _width(width), _height(height)
{
    _elementRegistry.initialize();
    _cells.resize(width * height,
        CellData { _elementRegistry.get_type_by_id("EMPTY"), 0 });
}

Simulation::Simulation(const Point &size, const ElementRegistry &elementRegistry)
    : Simulation(size.x, size.y, elementRegistry) { }

void Simulation::step()
{
    std::vector<CellData> next = _cells;
    
    for (int y = _height - 1; y >= 0; --y) {
        for (int x = 0; x < _width; ++x) {
            const int idx = flatten_coords(x, y);
            const ElementType *type = _cells[idx].type;  
            type->step_particle_at(this, x, y);            
        }
    }

    _cells = std::move(next);
}

void Simulation::set_type_at(const int x, const int y, const ElementType* type, const int colorIdx)
{
    if (x >= 0 && x < _width && y >= 0 && y < _height) { // TODO: Encapsulate this under a `within_bounds` func
        int idx = flatten_coords(x, y);
        _cells[idx].type = type;
        if (colorIdx > -1)
            _cells[idx].colorVariantIndex = colorIdx;
    }
}

void Simulation::set_type_at(const int x, const int y, const std::string &id, const int colorIdx)
{
    set_type_at(x, y, _elementRegistry.get_type_by_id(id), colorIdx);
}

const ElementType* Simulation::get_type_at(const int x, const int y) const
{
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        return _cells[flatten_coords(x, y)].type;
    }
    return nullptr;
}

void Simulation::fill_render_buffer(Color *dst) const
{
    const int total = _width * _height;
    for (int i = 0; i < total; ++i) {
        dst[i] = _cells[i].type->get_color(_cells[i].colorVariantIndex);
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
    return _elementRegistry.get_type_by_id(id);
}

std::vector<const ElementType *> Simulation::get_all_element_types() const
{
    return _elementRegistry.get_all_types();
}
