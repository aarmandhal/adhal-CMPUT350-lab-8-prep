#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    sf::Vector2f scale;
    sf::Vector2f translation;

    // Constructor - default constructor
    BasicTransform(sf::Vector2f scale = {1.0f, 1.0f}, sf::Vector2f translation = {0.0f, 0.0f});

    // applyPoint
    // applies the transform to a point
    sf::Vector2f applyPoint(const sf::Vector2f& p) const;

    // applyVector
    // applies the transform to a vector
    sf::Vector2f applyVector(const sf::Vector2f& v) const;

    // operator==
    // checks if two transforms are equal
    bool operator==(const BasicTransform& other) const;
};

// Constructor - default constructor
BasicTransform::BasicTransform(sf::Vector2f scale, sf::Vector2f translation) {
    this->scale = scale;
    this->translation = translation;
}

// applyPoint
// applies the transform to a point
sf::Vector2f BasicTransform::applyPoint(const sf::Vector2f& p) const {
    return p * scale + translation;
}

// applyVector
// applies the transform to a vector
sf::Vector2f BasicTransform::applyVector(const sf::Vector2f& v) const { return v * scale; }

// operator==
// checks if two transforms are equal
bool BasicTransform::operator==(const BasicTransform& other) const {
    return scale == other.scale && translation == other.translation;
}