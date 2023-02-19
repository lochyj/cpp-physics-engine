#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "utils/math.hpp"
#include <thread>
#include <future>

constexpr int32_t window_width = 1000;
constexpr int32_t window_height = 1000;

const bool isSpace = true;

struct VerletObject
{
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    float        radius        = 10.0f;
    sf::Color    color         = sf::Color::White;

    VerletObject() = default;
    VerletObject(sf::Vector2f position_, float radius_)
        : position{position_}
        , position_last{position_}
        , acceleration{0.0f, 0.0f}
        , radius{radius_}
    {}

    void update(float dt)
    {
        // Compute how much we moved
        const sf::Vector2f displacement = position - position_last;
        // Update position
        position_last = position;
        position      = position + displacement + acceleration * (dt * dt);
        // Reset acceleration
        acceleration = {};
    }

    void accelerate(sf::Vector2f a)
    {
        acceleration += a;
    }

    void setVelocity(sf::Vector2f v, float dt)
    {
        position_last = position - (v * dt);
    }

    void addVelocity(sf::Vector2f v, float dt)
    {
        position_last -= v * dt;
    }

    [[nodiscard]]
    sf::Vector2f getVelocity(float dt) const
    {
        return (position - position_last) / dt;
    }
};

std::vector<VerletObject> checkCollisions(std::vector<VerletObject> m_objects)
{
    const float    response_coef = 0.75f;
    const uint64_t objects_count = m_objects.size();
    // Iterate on all objects
    for (uint64_t i{ 0 }; i < objects_count; ++i) {
        VerletObject& object_1 = m_objects[i];
        // Iterate on object involved in new collision pairs
        for (uint64_t k{ i + 1 }; k < objects_count; ++k) {
            VerletObject& object_2 = m_objects[k];
            const sf::Vector2f v = object_1.position - object_2.position;
            const float        dist2 = v.x * v.x + v.y * v.y;
            const float        min_dist = object_1.radius + object_2.radius;
            // Check overlapping
            if (dist2 < min_dist * min_dist) {
                const float        dist = sqrt(dist2);
                const sf::Vector2f n = v / dist;
                const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
                const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
                const float delta = 0.5f * response_coef * (dist - min_dist);
                // Update positions
                object_1.position -= n * (mass_ratio_2 * delta);
                object_2.position += n * (mass_ratio_1 * delta);
            }
        }
    }
    return m_objects;
}

std::vector<VerletObject> applyGravity(std::vector<VerletObject> m_objects, sf::Vector2f m_gravity)
{
    if (isSpace)
    {
        float G = 0.81; // gravitational constant
        sf::Vector2f center(window_width / 2, window_height / 2);

        for (auto& obj : m_objects) {
            sf::Vector2f dir = center - obj.position;
            float dist = sqrt(G * (dir.x * dir.x) + G * (dir.y * dir.y)); // calculate distance using the Pythagorean theorem
            if (dist > 0) {
                dir.x /= dist;
                dir.y /= dist;
            }
            float force = G * (dist * dist);
            obj.accelerate(dir * force); // add outward force
        }
    } else
    {
        for (auto& obj : m_objects) {
            obj.accelerate(m_gravity);
        }
    }
    return m_objects;
}

void applyConstraint(std::vector<VerletObject> m_objects, sf::Vector2f m_constraint_center, float m_constraint_radius)
{
    for (auto& obj : m_objects) {
            const sf::Vector2f v    = m_constraint_center - obj.position;
            const float        dist = sqrt(v.x * v.x + v.y * v.y);
            if (dist > (m_constraint_radius - obj.radius)) {
                const sf::Vector2f n = v / dist;
                obj.position = m_constraint_center - n * (m_constraint_radius - obj.radius);
            }
    }
}

class Solver
{
public:
    Solver() = default;

    VerletObject& addObject(sf::Vector2f position, float radius)
    {
        m_objects.emplace_back(position, radius);
        return m_objects.back();
    }

    void update()
    {
        m_time += m_frame_dt;
        const float step_dt = getStepDt();
        for (uint32_t i{m_sub_steps}; i--;) {
            auto future = std::async(&checkCollisions, m_objects);
            m_objects = future.get();
            auto future2 = std::async(&checkCollisions, m_objects);
            m_objects = future2.get();
            m_objects = applyGravity(m_objects, m_gravity);
            applyConstraint(m_objects, m_constraint_center, m_constraint_radius);
            updateObjects(step_dt);
        }
        
    }

    void setSimulationUpdateRate(uint32_t rate)
    {
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void setConstraint(sf::Vector2f position, float radius)
    {
        m_constraint_center = position;
        m_constraint_radius = radius;
    }

    void setSubStepsCount(uint32_t sub_steps)
    {
        m_sub_steps = sub_steps;
    }

    void setObjectVelocity(VerletObject& object, sf::Vector2f v)
    {
        object.setVelocity(v, getStepDt());
    }

    [[nodiscard]]
    const std::vector<VerletObject>& getObjects() const
    {
        return m_objects;
    }

    [[nodiscard]]
    sf::Vector3f getConstraint() const
    {
        return {m_constraint_center.x, m_constraint_center.y, m_constraint_radius};
    }

    [[nodiscard]]
    uint64_t getObjectsCount() const
    {
        return m_objects.size();
    }

    [[nodiscard]]
    float getTime() const
    {
        return m_time;
    }

    [[nodiscard]]
    float getStepDt() const
    {
        return m_frame_dt / static_cast<float>(m_sub_steps);
    }

private:
    uint32_t                  m_sub_steps          = 10;
    sf::Vector2f              m_gravity            = {0.0f, -1000.0f};
    sf::Vector2f              m_constraint_center;
    float                     m_constraint_radius  = 100.0f;
    std::vector<VerletObject> m_objects;
    float                     m_time               = 0.0f;
    float                     m_frame_dt           = 0.0f;

    

    void updateObjects(float dt)
    {
        for (auto& obj : m_objects) {
            obj.update(dt);
        }
    }
};
