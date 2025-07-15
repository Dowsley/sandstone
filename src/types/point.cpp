//
// Created by João Dowsley on 14/07/25.
//

#include "point.h"

Point Point::operator+(const Point &other) const {
    return {X + other.X, Y + other.Y};
}

Point Point::operator-(const Point &other) const {
    return {X - other.X, Y - other.Y};
}

Point Point::operator*(const int scalar) const {
    return {X * scalar, Y * scalar};
}

Point Point::operator/(const int scalar) const {
    return {X / scalar, Y / scalar};
}

bool Point::operator==(const Point &other) const {
    return X == other.X && Y == other.Y;
}

bool Point::operator!=(const Point &other) const {
    return !(*this == other);
}
