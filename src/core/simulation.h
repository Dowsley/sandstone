//
// Created by João Dowsley on 14/07/25.
//

#ifndef SIMULATION_H
#define SIMULATION_H

#include <raylib.h>
#include <vector>

#include "../types/vector2i.h"
#include "../elements/element_registry.h"
#include "cell_matrix.h"

class Simulation {
public:
    Simulation(int width, int height, ElementRegistry& element_registry);
    explicit Simulation(const Vector2I &size, ElementRegistry& element_registry);
    ~Simulation() = default;

    const Vector2I UP = Vector2I(0, -1);
    const Vector2I DOWN = Vector2I(0, 1);
    const Vector2I RIGHT = Vector2I(1, 0);
    const Vector2I LEFT = Vector2I(-1, 0);

    void step();

    bool set_type_at(int x, int y, const ElementType *type, int color_idx = -1);
    bool set_type_at(const Vector2I &pos, const ElementType *type, int color_idx = -1);
    bool set_type_at(int x, int y, const std::string &id, int color_idx = -1);
    bool set_type_at(const Vector2I &pos,  const std::string &id, int color_idx = -1);
    const ElementType* get_type_at(int x, int y) const;
    const ElementType* get_type_at(const Vector2I &pos) const;
    void fill_render_buffer(Color *dst) const;
    void fill_temperature_buffer(Color *dst, Color cold, Color hot, int t_min = 0, int t_max = 1000) const;

    int get_width() const;
    int get_height() const;

    int flatten_coords(int x, int y) const;
    int flatten_coords(const Vector2I &pos) const;

    ElementType* get_type_by_id(const std::string &id) const;
    std::vector<const ElementType*> get_all_element_types() const;
    bool is_pos_empty(const Vector2I &pos) const;
    bool is_pos_empty(int x, int y) const;
    bool is_pos_within_bounds(const Vector2I &pos) const;
    bool is_pos_within_bounds(int x, int y) const;

private:
    ElementRegistry& _element_registry;
    
    int _width;
    int _height;
    int _step_count = 0;

    CellMatrix _cells;
    CellMatrix _next_cells;
};

#endif //SIMULATION_H
