//
// Created by João Dowsley on 15/07/25.
//

#ifndef ELEMENT_REGISTRY_H
#define ELEMENT_REGISTRY_H

#include "element_type.h"
#include "element_loader.h"
#include "../core/abstract/base_registry.h"


class ElementRegistry final : public BaseRegistry<ElementType, ElementLoader> {
public:
    using BaseRegistry::BaseRegistry; // Inherit constructors

protected:
    std::vector<ElementType*> _load_specific() override;
};

#endif //ELEMENT_REGISTRY_H
