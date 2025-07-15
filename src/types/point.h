//
// Created by João Dowsley on 14/07/25.
//

#ifndef POINT_H
#define POINT_H

struct Point
{
    int X;
    int Y;

    Point() : X(0), Y(0) {}
    Point(const int x, const int y) : X(x), Y(y) {}

    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const;
    Point operator*(int scalar) const;
    Point operator/(int scalar) const;

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
};

#endif //POINT_H
