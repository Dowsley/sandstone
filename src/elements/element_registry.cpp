//
// Created by João Dowsley on 15/07/25.
//

#include "element_registry.h"

#include "empty.h"
#include "types/movable_solid.h"
#include "types/immovable_solid.h"
#include "types/liquid.h"

std::vector<ElementType*> ElementRegistry::_load_specific() {
    // return loader.LoadAll(); // TODO: Add data-driven XML stuff

    std::vector<ElementType*> elements;

    const auto empty = (new Empty())
        ->set_id("EMPTY")
        ->set_name("Empty")
        ->set_description("Nothing. Nil.")
        ->set_density(0)
        ->add_color_variant({ 0, 0, 0, 0 });
    elements.push_back(empty);

    const auto sand = (new MovableSolid())
        ->set_id("SAND")
        ->set_name("Sand")
        ->set_description("This is the sand element.")
        ->set_density(10)
        ->add_color_variant({ 240, 213, 129, 255 })
        ->add_color_variant({ 232, 191, 63, 255 })
        ->add_color_variant({ 248, 235, 195, 255 });
    elements.push_back(sand);

    const auto stone = (new ImmovableSolid())
        ->set_id("STONE")
        ->set_name("Stone")
        ->set_description("This is the stone element.")
        ->set_density(100)
        ->add_color_variant(GRAY)
        ->add_color_variant({ 104, 104, 104, 255 })
        ->add_color_variant({ 156, 156, 156, 255 });
    elements.push_back(stone);

    const auto water = (new Liquid())
        ->set_id("WATER")
        ->set_name("Water")
        ->set_description("This is the water element.")
        ->set_density(1)
        // ->add_color_variant({ 60,128,242, 255 });
        ->add_color_variant({ 15,93,226, 255 });
        // ->add_color_variant({ 117, 164, 246, 255 });
    elements.push_back(water);
    
    return elements;
}

