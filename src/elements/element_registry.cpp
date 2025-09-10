//
// Created by João Dowsley on 15/07/25.
//

#include "element_registry.h"

std::vector<ElementType*> ElementRegistry::_load_specific() {
    return loader.load_all();
}

