//
// Created by João Dowsley on 09/09/25.
//

#ifndef SANDSTONE_ELEMENT_UTIL_H
#define SANDSTONE_ELEMENT_UTIL_H

#include "../elements/element_type.h"


class ElementTypeChecker {
public:
    static bool is_empty(const ElementType& element);
    static bool is_liquid(const ElementType& element);
    static bool is_immovable_solid(const ElementType& element);
    static bool is_movable_solid(const ElementType& element);
    static bool is_solid(const ElementType& element);
    static bool is_gas(const ElementType& element);
};

#endif //SANDSTONE_ELEMENT_UTIL_H