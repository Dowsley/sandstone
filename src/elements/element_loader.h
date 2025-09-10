//
// Created by João Dowsley on 15/07/25.
//

#ifndef ELEMENT_LOADER_H
#define ELEMENT_LOADER_H

#include <string>

#include "element_type.h"
#include "../core/abstract/base_loader.h"

class ElementLoader final : public BaseLoader<ElementType> {
public:
    explicit ElementLoader(const std::string &path) : BaseLoader(path) {}
    std::vector<ElementType*> load_all() override;

protected:
    ElementType* _load_specific(const std::string &file) override;
};



#endif //ELEMENT_LOADER_H
