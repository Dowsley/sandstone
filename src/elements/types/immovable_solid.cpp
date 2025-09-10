//
// Created by João Dowsley on 15/07/25.
//

#include "immovable_solid.h"

ImmovableSolid::ImmovableSolid()
{
    _kind = ElementKind::ImmovableSolid;
}

bool ImmovableSolid::step_particle_at(
    CellMatrix &curr_cells,
    CellMatrix &next_cells,
    const int x, const int y, const ElementType *type) const
{
    // No movement. Nothing.
    return true;
}