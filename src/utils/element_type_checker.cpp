//
// Created by João Dowsley on 09/09/25.
//

#include "element_type_checker.h"
#include "../elements/element_type.h"

bool ElementTypeChecker::is_empty(const ElementType &element) noexcept
{
    return element.get_kind() == ElementKind::Empty;
}

bool ElementTypeChecker::is_of_kind(const ElementType &element, const ElementKind kind) noexcept
{
    return element.get_kind() == kind;
}

bool ElementTypeChecker::is_of_kinds(const ElementType &element, const std::initializer_list<ElementKind> kinds) noexcept
{
    const ElementKind k = element.get_kind();
    for (const auto kk : kinds) {
        if (kk == k) return true;
    }
    return false;
}
