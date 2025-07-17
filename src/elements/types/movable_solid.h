//
// Created by João Dowsley on 15/07/25.
//

#ifndef MOVABLE_SOLID_H
#define MOVABLE_SOLID_H

#include "abstract/solid.h"

class Simulation;

class MovableSolid final : public Solid {
public:
    bool step_particle_at(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y, const ElementType *type) const override;
};


#endif //MOVABLE_SOLID_H
