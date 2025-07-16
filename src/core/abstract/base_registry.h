//
// Created by João Dowsley on 15/07/25.
//

#ifndef REGISTRY_H
#define REGISTRY_H

#include <unordered_map>
#include <string>
#include <vector>

template <typename T, typename L>
class BaseRegistry {
public:
    explicit BaseRegistry(const std::string &path)
            : loader(path) {
        // Do not call _load() here
    }

    virtual ~BaseRegistry() {
        for (auto & type : types) {
            delete type.second;
        }
    }

    void reload() {
        types.clear();
        _load();
    }

    void initialize() {
        _load();
    }

    T* get_type_by_id(const std::string &id) const {
        auto it = types.find(id);
        if (it != types.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::vector<const T*> get_all_types() const {
        std::vector<const T*> vec;
        for (const auto& pair : types) {
            vec.push_back(pair.second);
        }
        return vec;
    }

protected:
    void _load() {
        for (const auto & type : _load_specific()) {
            types[type->get_id()] = type;
        }
    }

    virtual std::vector<T*> _load_specific() = 0;

    L loader;
    std::unordered_map<std::string, T*> types;
};

#endif //REGISTRY_H
