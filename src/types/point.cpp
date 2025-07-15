//
// Created by João Dowsley on 14/07/25.
//

#include "point.h"

Point Point::operator+(const Point &other) const {
    return {x + other.x, y + other.y};
}

Point Point::operator-(const Point &other) const {
    return {x - other.x, y - other.y};
}

Point Point::operator*(const int scalar) const {
    return {x * scalar, y * scalar};
}

Point Point::operator/(const int scalar) const {
    return {x / scalar, y / scalar};
}

bool Point::operator==(const Point &other) const {
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point &other) const {
    return !(*this == other);
}
