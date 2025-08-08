//
// Created by João Dowsley on 14/07/25.
//

#include "vector2i.h"

Vector2I Vector2I::operator+(const Vector2I &other) const {
    return {x + other.x, y + other.y};
}

Vector2I Vector2I::operator-(const Vector2I &other) const {
    return {x - other.x, y - other.y};
}

Vector2I Vector2I::operator*(const int scalar) const {
    return {x * scalar, y * scalar};
}

Vector2I Vector2I::operator/(const int scalar) const {
    return {x / scalar, y / scalar};
}

bool Vector2I::operator==(const Vector2I &other) const {
    return x == other.x && y == other.y;
}

bool Vector2I::operator!=(const Vector2I &other) const {
    return !(*this == other);
}
