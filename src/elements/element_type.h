//
// Created by João Dowsley on 15/07/25.
//

#ifndef ELEMENT_TYPE_H
#define ELEMENT_TYPE_H

#include "raylib.h"

#include <list>
#include <string>
#include <vector>

class CellMatrix;

enum class ElementKind {
    Unknown = 0,
    Empty,
    Liquid,
    MovableSolid,
    ImmovableSolid,
    Gas
};

class ElementType {
protected:
    std::string _id;
    std::string _name;
    std::string _description;
    int _density = 0;
    std::vector<Color> _color_variants;
    ElementKind _kind = ElementKind::Unknown;

public:
    virtual ~ElementType() = default;
    const std::string& get_id() const;
    const std::string& get_name() const;
    const std::string& get_description() const;
    int get_density() const;
    const std::vector<Color>& get_color_variants() const;
    ElementKind get_kind() const { return _kind; }

    const Color& get_color(int index) const;
    int get_random_color_index() const;

    // Builder pattern!
    ElementType* set_id(const std::string &id);
    ElementType* set_description(const std::string &description);
    ElementType* set_name(const std::string &name);
    ElementType* set_density(int density);
    ElementType* add_color_variant(const Color &colorVariant);
    ElementType* set_kind(ElementKind kind) { _kind = kind; return this; }

    virtual bool step_particle_at(
       CellMatrix &curr_cells,
       CellMatrix &next_cells,
    int x, int y, const ElementType *type) const = 0;
};

#endif //ELEMENT_TYPE_H
