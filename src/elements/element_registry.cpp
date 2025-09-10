//
// Created by João Dowsley on 15/07/25.
//

#include "element_registry.h"

#include "empty.h"
#include "types/gas.h"
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
        ->set_density(5)
        ->add_color_variant({ 0, 0, 0, 0 });
    elements.push_back(empty);

    const auto sand = (new MovableSolid())
        ->set_id("SAND")
        ->set_name("Sand")
        ->set_description("This is the sand element.")
        ->set_density(100)
        ->add_color_variant({ 238, 221, 126, 255 })
        ->add_color_variant({ 222, 205, 111, 255 })
        ->add_color_variant({ 232, 215, 121, 255 })
        ->add_color_variant({ 230, 213, 119, 255 })
        ->add_color_variant({ 221, 204, 110, 255 })
        ->add_color_variant({ 241, 224, 129, 255 });
    elements.push_back(sand);

    const auto gravel = (new MovableSolid())
        ->set_id("GRAVEL")
        ->set_name("Gravel")
        ->set_description("Small stones that are heavier than sand.")
        ->set_density(120)
        ->add_color_variant({ 130, 120, 110, 255 })
        ->add_color_variant({ 110, 100, 90, 255 })
        ->add_color_variant({ 140, 130, 120, 255 });
    elements.push_back(gravel);

    const auto stone = (new ImmovableSolid())
        ->set_id("STONE")
        ->set_name("Stone")
        ->set_description("This is the stone element.")
        ->set_density(1000)
        ->add_color_variant(GRAY)
        ->add_color_variant({ 104, 104, 104, 255 })
        ->add_color_variant({ 156, 156, 156, 255 });
    elements.push_back(stone);

    const auto metal = (new ImmovableSolid())
        ->set_id("METAL")
        ->set_name("Metal")
        ->set_description("Heavy, immovable block.")
        ->set_density(3000)
        ->add_color_variant({ 180, 180, 190, 255 })
        ->add_color_variant({ 160, 160, 170, 255 });
    elements.push_back(metal);

    const auto water = (new Liquid())
        ->set_id("WATER")
        ->set_name("Water")
        ->set_description("This is the water element.")
        ->set_density(50)
        // ->add_color_variant({ 60,128,242, 255 });
        ->add_color_variant({ 15,93,226, 255 });
        // ->add_color_variant({ 117, 164, 246, 255 });
    elements.push_back(water);

    const auto oil = (new Liquid())
        ->set_id("OIL")
        ->set_name("Oil")
        ->set_description("Viscous liquid lighter than water.")
        ->set_density(30)
        ->add_color_variant({ 60, 50, 20, 255 })
        ->add_color_variant({ 70, 55, 25, 255 });
    elements.push_back(oil);

    const auto steam = (new Gas())
        ->set_id("STEAM")
        ->set_name("Steam")
        ->set_description("This is the steam element.")
        ->set_density(1)
        ->add_color_variant({ 255/2, 255/2, 255/2, 255 });
    elements.push_back(steam);
    
    const auto chlorine = (new Gas())
        ->set_id("CHLORINE")
        ->set_name("Chlorine")
        ->set_description("Heavier-than-air gas that tends to sink.")
        ->set_density(8)
        ->add_color_variant({ 120, 200, 80, 200 });
    elements.push_back(chlorine);
    
    return elements;
}

