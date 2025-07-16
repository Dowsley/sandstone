//
// Created by João Dowsley on 14/07/25.
//

#ifndef SIMULATION_H
#define SIMULATION_H

#include <raylib.h>
#include <vector>

#include "../structures/point.h"
#include "../elements/element_registry.h"

struct CellData {
    const ElementType* type; // we can change it to point elsewhere… but not change the instance itself
    uint8_t colorVariantIndex = 0;
};

class Simulation {
public:
    Simulation(int width, int height, const ElementRegistry &elementRegistry);
    explicit Simulation(const Point &size, const ElementRegistry &elementRegistry);
    ~Simulation() = default;

    const Point UP = Point(0, -1);
    const Point DOWN = Point(0, 1);
    const Point RIGHT = Point(1, 0);
    const Point LEFT = Point(-1, 0);

    void step();

    void set_type_at(int x, int y, const ElementType *type, int colorIdx = -1);
    void set_type_at(int x, int y, const std::string &id, int colorIdx = -1);
    const ElementType* get_type_at(int x, int y) const;
    void fill_render_buffer(Color *dst) const;

    int get_width() const;
    int get_height() const;

    int flatten_coords(int x, int y) const;
    int flatten_coords(const Point &pos) const;

    ElementType* get_type_by_id(const std::string &id) const;
    std::vector<const ElementType*> get_all_element_types() const;

private:
    ElementRegistry _elementRegistry;
    
    int _width;
    int _height;

    std::vector<CellData> _cells;
};

#endif //SIMULATION_H
