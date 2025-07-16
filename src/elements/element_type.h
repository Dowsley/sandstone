//
// Created by João Dowsley on 15/07/25.
//

#ifndef ELEMENT_TYPE_H
#define ELEMENT_TYPE_H
#include "raylib.h"
// #include "../core/simulation.h"
class Simulation;

#include <list>
#include <string>


class ElementType {
protected:
    std::string _id;
    std::string _name;
    std::string _description;
    int _density = 0;
    // int _melting_point;
    // int _boiling_point;
    std::vector<Color> _color_variants;

public:
    const std::string& get_id() const;
    const std::string& get_name() const;
    const std::string& get_description() const;
    int get_density() const;
    const std::vector<Color>& get_color_variants() const;

    const Color& get_color(int index) const;
    int get_random_color_index() const;

    // Builder pattern!
    ElementType* set_id(const std::string &id);
    ElementType* set_description(const std::string &description);
    ElementType* set_name(const std::string &name);
    ElementType* set_density(int density);
    ElementType* add_color_variant(const Color &colorVariant);

    // TODO: Consider encapsulating the eventual multiple systems in the simulation and pass them separately here.
    virtual bool step_particle_at(Simulation *sim, int x, int y) const;
};

#endif //ELEMENT_TYPE_H
