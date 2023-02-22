#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include<stdio.h>

#include "objects.hpp"

// TODO: Make this use threading and use the updated version of collision detection using a grid later on...
std::vector<object> calculate_collisions(std::vector<object> objects) {
    const float response_coefficient = 0.75f;
    const int   objects_count = objects.size();

    // Iterate on all objects
    for (int i{ 0 }; i < objects_count; ++i) {
        object& obj1 = objects[i];

        // Iterate on objects involved in new collision pairs
        for (int k{ i + 1 }; k < objects_count; ++k) {
            object& obj2 = objects[k];
            const sf::Vector2f v = obj1.position - obj2.position;
            const float        dist2 = v.x * v.x + v.y * v.y;
            const float        min_dist = obj1.radius + obj2.radius;

            // Check if the objects are overlapping
            if (dist2 < min_dist * min_dist) {
                const float         dist = sqrt(dist2);
                const sf::Vector2f     n = v / dist;
                const float mass_ratio_1 = obj1.radius / (obj1.radius + obj2.radius);
                const float mass_ratio_2 = obj2.radius / (obj1.radius + obj2.radius);
                const float        delta = 0.5f * response_coefficient * (dist - min_dist);

                // Update object positions
                obj1.position -= n * (mass_ratio_2 * delta);
                obj2.position += n * (mass_ratio_1 * delta);
            }
        }
    }
    return objects;
}

// Gravity - check gravity.png for more info | m1 is the center / black hole gravity object
// Fg = G((m1m2)/r^2)
// G = 0.81

// Acceleration
// F = ma

bool space = false;

sf::Vector2f gravity = sf::Vector2f(0.0f, 100000.0f);

std::vector<object> process_gravity(std::vector<object> objects) {
    if (space) {
        float G = 0.81; // gravitational constant
        sf::Vector2f center(500.0f, 500.0f);
        for (auto& obj : objects) {
            /*// Temp
            sf::Vector2f center = { 500.0f, 500.0f };
            float m1 = 1.0f;

            // Make global
            const float G = 6.67e-11f;

            sf::Vector2f r = center - obj.position;
            float m2 = obj.mass;

            // This is equal to Fg = G((m1m2)/r^2), where Fg is objAcceleration
            auto objAcceleration = G *
                sf::Vector2f(
                    pow(r.x, 2) / (m1 * m2),
                    pow(r.y, 2) / (m1 * m2)
                );
            //printf(" x: %f, y: %f", obj.position.x, obj.position.y);
            obj.accelerate(objAcceleration);
            */

            sf::Vector2f dir = center - obj.position;
            float dist = sqrt(G * (dir.x * dir.x) + G * (dir.y * dir.y)); // calculate distance using the Pythagorean theorem
            if (dist > 0) {
                dir.x /= dist;
                dir.y /= dist;
            }
            float force = G * (dist * dist);
            obj.accelerate(dir * force); // add outward force
        }
    }
    else {
        for (auto& obj : objects) {
            obj.accelerate(gravity);
        }
    }
    
    return objects;
}

// TODO: make this not a circle in the future
std::vector<object> apply_boundaries(std::vector<object> objects) {
    sf::Vector2f center = { 500.0f, 500.0f };
    float radius = 500.0f;
    float boundaries_friction = 100.0f;
    for (auto& obj : objects) {
        
        // R is the distance from the center of the container to the center of the object
        const sf::Vector2f  R = center - obj.position;
        const float         dist = sqrt(R.x * R.x + R.y * R.y);

        if (dist > (radius - obj.radius)) {
            const sf::Vector2f  n = R / dist;
            //obj.acceleration = sf::Vector2f(obj.friction_constant - boundaries_friction / obj.acceleration.x, obj.friction_constant - boundaries_friction / obj.acceleration.y);
            obj.position = center - n * (radius - obj.radius);
        }

    }

    return objects;
}

std::vector<object> update(std::vector<object> objects, int iterations, std::vector<grid> Grid) {
    for (int i = iterations; i > 0; --i) {
        objects = calculate_collisions(objects);
    }
    objects = process_gravity(objects);
    objects = apply_boundaries(objects);
    for (auto& obj : objects) {
        obj.updatePosition(0.002f, Grid);
    }
    return objects;
}
