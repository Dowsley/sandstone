//
// Created by João Dowsley on 08/08/25.
//

#ifndef SANDSTONE_GAS_H
#define SANDSTONE_GAS_H

#include "../element_type.h"

class Gas final : public ElementType
{
public:
    bool step_particle_at(
        CellMatrix &curr_cells,
        CellMatrix &next_cells,
        int x, int y, const ElementType *type) const override;
};


#endif //SANDSTONE_GAS_H