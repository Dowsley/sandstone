//
// Created by João Dowsley on 09/09/25.
//

#ifndef SANDSTONE_ELEMENT_UTIL_H
#define SANDSTONE_ELEMENT_UTIL_H

#include "../elements/element_type.h"


class ElementTypeChecker {
public:
    static bool is_empty(const ElementType& element) noexcept;
    static bool is_of_kind(const ElementType& element, ElementKind kind) noexcept;
    static bool is_any_of_kinds(const ElementType& element, std::initializer_list<ElementKind> kinds) noexcept;
};

#endif //SANDSTONE_ELEMENT_UTIL_H