//
// Created by João Dowsley on 09/09/25.
//

#include "element_type_checker.h"

#include "../elements/types/liquid.h"
#include "../elements/types/immovable_solid.h"
#include "../elements/types/movable_solid.h"
#include "../elements/types/gas.h"

bool ElementTypeChecker::is_empty(const ElementType &element)
{
    return element.get_id() == "EMPTY";
}

bool ElementTypeChecker::is_liquid(const ElementType &element)
{
    try {
        [[maybe_unused]] auto &gasRef = dynamic_cast<Liquid&>(const_cast<ElementType&>(element));
        return true;
    } catch ([[maybe_unused]] const std::bad_cast& e) {
        return false;
    }
}

bool ElementTypeChecker::is_immovable_solid(const ElementType &element)
{
    try {
        [[maybe_unused]] auto &gasRef = dynamic_cast<ImmovableSolid&>(const_cast<ElementType&>(element));
        return true;
    } catch ([[maybe_unused]] const std::bad_cast& e) {
        return false;
    }
}

bool ElementTypeChecker::is_movable_solid(const ElementType &element)
{
    try {
        [[maybe_unused]] auto &gasRef = dynamic_cast<MovableSolid&>(const_cast<ElementType&>(element));
        return true;
    } catch ([[maybe_unused]] const std::bad_cast& e) {
        return false;
    }
}

bool ElementTypeChecker::is_solid(const ElementType &element)
{
    return is_immovable_solid(element) || is_movable_solid(element);
}

bool ElementTypeChecker::is_gas(const ElementType &element)
{
    try {
        [[maybe_unused]] auto &gasRef = dynamic_cast<Gas&>(const_cast<ElementType&>(element));
        return true;
    } catch ([[maybe_unused]] const std::bad_cast& e) {
        return false;
    }
}
