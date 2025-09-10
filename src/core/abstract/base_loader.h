//
// Created by João Dowsley on 15/07/25.
//

#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <utility>
#include <vector>
#include <filesystem>

template<typename T>
class BaseLoader {
public:
    explicit BaseLoader(std::string path) : _directory_path(std::move(path)) {}

    virtual ~BaseLoader() = default;

    virtual std::vector<T*> load_all() = 0;

protected:
    virtual T* _load_specific(const std::string &file) = 0;
    std::string _directory_path;
};

#endif //LOADER_H
