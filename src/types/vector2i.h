//
// Created by João Dowsley on 14/07/25.
//

#ifndef POINT_H
#define POINT_H

#include <raylib.h>

struct Vector2I
{
    int x;
    int y;

    Vector2I() : x(0), y(0) {}
    Vector2I(const int x, const int y) : x(x), y(y) {}
    explicit Vector2I(const Vector2 vec) : x(vec.x), y(vec.y) {}

    Vector2I operator+(const Vector2I& other) const;
    Vector2I operator-(const Vector2I& other) const;
    Vector2I operator*(int scalar) const;
    Vector2I operator/(int scalar) const;

    bool operator==(const Vector2I& other) const;
    bool operator!=(const Vector2I& other) const;
};

#endif //POINT_H
