//
// Created by João Dowsley on 16/07/25.
//

#ifndef EMPTY_H
#define EMPTY_H
#include "element_type.h"

class Simulation;

class Empty final : public ElementType {
public:
    Empty() { _kind = ElementKind::Empty; }
    bool step_particle_at(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y, const ElementType *type) const override;
};




#endif //EMPTY_H
