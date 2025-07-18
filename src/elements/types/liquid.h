//
// Created by João Dowsley on 15/07/25.
//

#ifndef LIQUID_H
#define LIQUID_H

#include "../element_type.h"

class Simulation;

class Liquid final : public ElementType {
protected:
    int _boiling_point = 0;
    int _freezing_point = 0;
    // int _viscosity = 0; // TODO: Consider how to implement this. Maybe only processing every N frames or so?!

public:
    bool step_particle_at(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y, const ElementType *type) const override;
};



#endif //LIQUID_H
