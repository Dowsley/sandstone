//
// Created by João Dowsley on 15/07/25.
//

#include "element_loader.h"
#include <vector>
#include <string>
#include <pugixml.hpp>
#include <filesystem>
#include "types/liquid.h"
#include "types/movable_solid.h"
#include "types/immovable_solid.h"
#include "types/gas.h"
#include "empty.h"

static ElementType* create_by_kind(const std::string &kind)
{
    if (kind == "Empty") return new Empty();
    if (kind == "Liquid") return new Liquid();
    if (kind == "MovableSolid") return new MovableSolid();
    if (kind == "ImmovableSolid") return new ImmovableSolid();
    if (kind == "Gas") return new Gas();
    return nullptr;
}

static unsigned char to_u8(const int v, const int def = 0)
{
    int x = v;
    if (x < 0) x = def;
    if (x > 255) x = 255;
    return static_cast<unsigned char>(x);
}

ElementType* ElementLoader::_load_specific(const std::string &file) {
    pugi::xml_document doc;
    const pugi::xml_parse_result ok = doc.load_file(file.c_str());
    if (!ok) return nullptr;

    const pugi::xml_node n = doc.child("Element");
    if (!n) return nullptr;

    const char* id_c   = n.attribute("id").as_string(nullptr);
    const char* name_c = n.attribute("name").as_string(nullptr);
    const char* kind_c = n.attribute("kind").as_string(nullptr);
    const int density  = n.attribute("density").as_int(0);
    if (!id_c || !name_c || !kind_c) return nullptr;

    ElementType* t = create_by_kind(kind_c);
    if (!t) return nullptr;

    const char* desc_c = n.child("Description").text().as_string("");
    t->set_id(id_c)
     ->set_name(name_c)
     ->set_description(desc_c)
     ->set_density(density);

    for (const pugi::xml_node c : n.children("Color")) {
        Color col {
            to_u8(c.attribute("r").as_int(0)),
            to_u8(c.attribute("g").as_int(0)),
            to_u8(c.attribute("b").as_int(0)),
            to_u8(c.attribute("a").as_int(255))
        };
        t->add_color_variant(col);
    }

    return t;
}

std::vector<ElementType*> ElementLoader::load_all() {
    std::vector<ElementType*> out;

    std::error_code ec;
    const std::filesystem::path dir(_directory_path);
    if (!std::filesystem::exists(dir, ec)) return out;
    if (!std::filesystem::is_directory(dir, ec)) return out;

    for (const auto &entry : std::filesystem::directory_iterator(dir, ec)) {
        if (ec) break;
        if (!entry.is_regular_file()) continue;
        const auto &path = entry.path();
        if (path.extension() != ".xml") continue;

        if (ElementType* t = _load_specific(path.string())) {
            out.push_back(t);
        }
    }

    return out;
}