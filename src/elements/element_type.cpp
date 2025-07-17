//
// Created by João Dowsley on 15/07/25.
//

#include "element_type.h"
#include "../core/cell_data.h"
#include "../core/simulation.h"

const std::string& ElementType::get_id() const { return _id; }
const std::string& ElementType::get_description() const { return _description; }
const std::string& ElementType::get_name() const { return _name; }
int ElementType::get_density() const { return _density; }
const std::vector<Color>& ElementType::get_color_variants() const { return _color_variants; }

const Color& ElementType::get_color(const int index) const { return _color_variants[index]; }

// TODO: Better randomize this with a random util lib
int ElementType::get_random_color_index() const { return rand() % _color_variants.size(); }

ElementType* ElementType::set_id(const std::string &id) { this->_id = id; return this; }
ElementType* ElementType::set_description(const std::string &description) { this->_description = description; return this; }
ElementType* ElementType::set_name(const std::string &name) { this->_name = name; return this; }
ElementType* ElementType::set_density(const int density) { this->_density = density; return this; }
ElementType* ElementType::add_color_variant(const Color &colorVariant) { this->_color_variants.push_back(colorVariant); return this; }
