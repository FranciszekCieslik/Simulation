#include <SFML/Graphics.hpp>
#include <cmath>

namespace my_sp
{
    float getViscosity(sf::Vector2f position)
    {
        float x = position.x - 400;
        float y = position.y - 400;
        if(position.x < 400) return 0;
        return 0.01f + 0.00005f * sqrt(x * x + y * y);
    }

    int v_multi(int a, int b)
    {
        int c = a * a + b * b;
        c = sqrt(c);
        return c;
    };

    int v_multi(sf::Vector2f v)
    {
        int c = v.x * v.x + v.y * v.y;
        c = sqrt(c);
        return c;
    };

    sf::Vector2f v_add(sf::Vector2f v, sf::Vector2f u, float d)
    {
        return sf::Vector2f((v.x + u.x * d), (v.y + u.y * d));
    };

    class Circle
    {

    private:
        int mass, radius;

    public:
        sf::CircleShape shape;
        sf::Vector2f velocity;

    public:
        Circle()
        {
            radius = (rand() % 20) + 20;
            mass = rand() % 10;
            velocity = sf::Vector2f(rand() % 9 - rand() % 10 + 1, rand() % 9 - rand() % 10 + 1);
            shape.setRadius(radius);
            shape.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
            shape.setOrigin(radius, radius);
        };

        int get_mass()
        {
            return mass;
        };

        int get_radius()
        {
            return radius;
        };

        void setPosition(sf::Vector2f pos)
        {
            shape.setPosition(pos);
        }

        sf::Vector2f getPosition()
        {
            return shape.getPosition();
        }

        sf::Vector2f acceleration(sf::Vector2f Pos, float scal)
        {
            float x = Pos.x - shape.getPosition().x,
                  y = Pos.y - shape.getPosition().y,
                  r2 = (x * x + y * y);
            if (abs(r2) < 1e-8)
                r2 = 1e-8;
            float r = sqrt(r2),
                  a = scal * (1 / r2), ay = (a * y) / r, ax = (a * x) / r;
            return sf::Vector2f(ax, ay);
        }

        sf::Vector2f dragForce(float viscosity)
        {
            
            float dragMagnitude = 6 * 3.14159 * viscosity * radius;
            return sf::Vector2f(-dragMagnitude * velocity.x, -dragMagnitude * velocity.y);
        }

        sf::Vector2f dragForce()
        {
            float viscosity = getViscosity(shape.getPosition());
            float dragMagnitude = 6 * 3.14159 * viscosity * radius ;
            return sf::Vector2f(-dragMagnitude * velocity.x, -dragMagnitude * velocity.y);
        }
    };
}
