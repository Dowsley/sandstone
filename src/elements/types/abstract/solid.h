//
// Created by João Dowsley on 15/07/25.
//

#ifndef SOLID_H
#define SOLID_H

#include "../../element_type.h"

class Simulation;

class Solid : public ElementType {
protected:
    int _melting_point = 0;
};



#endif //SOLID_H
