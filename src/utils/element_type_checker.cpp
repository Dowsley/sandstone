//
// Created by João Dowsley on 09/09/25.
//

#include "element_type_checker.h"

#include "../elements/types/liquid.h"
#include "../elements/types/immovable_solid.h"
#include "../elements/types/movable_solid.h"
#include "../elements/types/gas.h"

bool ElementTypeChecker::is_empty(const ElementType &element) noexcept
{
    return element.get_id() == "EMPTY";
}

bool ElementTypeChecker::is_liquid(const ElementType &element) noexcept
{
    return dynamic_cast<const Liquid*>(&element) != nullptr;
}

bool ElementTypeChecker::is_immovable_solid(const ElementType &element) noexcept
{
    return dynamic_cast<const ImmovableSolid*>(&element) != nullptr;
}

bool ElementTypeChecker::is_movable_solid(const ElementType &element) noexcept
{
    return dynamic_cast<const MovableSolid*>(&element) != nullptr;
}

bool ElementTypeChecker::is_solid(const ElementType &element) noexcept
{
    return is_immovable_solid(element) || is_movable_solid(element);
}

bool ElementTypeChecker::is_gas(const ElementType &element) noexcept
{
    return dynamic_cast<const Gas*>(&element) != nullptr;
}
