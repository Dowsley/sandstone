//
// Created by João Dowsley on 14/07/25.
//

#ifndef POINT_H
#define POINT_H

#include <raylib.h>

struct Point
{
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(const int x, const int y) : x(x), y(y) {}
    explicit Point(const Vector2 vec) : x(vec.x), y(vec.y) {}

    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const;
    Point operator*(int scalar) const;
    Point operator/(int scalar) const;

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
};

#endif //POINT_H
