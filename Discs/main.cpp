#include <SFML/Graphics.hpp>
#include <vector>
#include "MyCircle.cpp"

int main()
{
    // create window
    constexpr int window_width{800}, window_height{800}, fps{60};
    int r, index;
    sf::Vector2f position, dragForceVal, main_mass_pos = sf::Vector2f(400,400);
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Simulation Part 1");
    window.setFramerateLimit(fps);
    // create shapes
    std::vector<my_sp::Circle> circlesVector;
    circlesVector.reserve(100);

    {
        for (int i{0}; i < 100; i++)
        {
            my_sp::Circle circle;

            circle.setPosition(sf::Vector2f(rand() % (window_width - 120) + 60, rand() % (window_width - 120) + 60));

            // set start shapes properities
            circlesVector.push_back(circle);
        }
    }

    // view loop
    while (window.isOpen())
    {
        // exit event
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // inputs control
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
            {
                // Pobranie pozycji myszy względem okna
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                main_mass_pos = window.mapPixelToCoords(mousePosition);   
            }
        }

        index = 0;
        // logic
        for (my_sp::Circle &circle : circlesVector)
        {
            position = circle.getPosition();
            r = circle.get_radius();

            // border collision
            if (position.x > (window_width - (r)))
            {
                circle.velocity.x *= (-1);
                position.x = window_width - r;
            }
            else if (position.x < r)
            {
                circle.velocity.x *= (-1);
                position.x = r;
            };

            if (position.y > (window_height - r))
            {
                circle.velocity.y *= (-1);
                position.y = window_height - r;
            }
            else if (position.y < r)
            {
                circle.velocity.y *= (-1);
                position.y = r;
            };
            // set new position
            dragForceVal = circle.dragForce();
            circle.velocity += circle.acceleration(main_mass_pos, 800);
            circle.velocity = my_sp::v_add(circle.velocity, dragForceVal, 0.01);
            circle.shape.setPosition(position.x + circle.velocity.x, position.y + circle.velocity.y);

            // deleting too fast object to keeep clarity of view
            if (my_sp::v_multi(circle.velocity) > 70)
            {
                //circlesVector.erase(circlesVector.begin() + index);
                circle.velocity.x = 10;
                circle.velocity.y = 10;
            }
            else
            {
                index++;
            }
        }

        // rendering
        window.clear();
        for (my_sp::Circle circle : circlesVector)
        {
            window.draw(circle.shape);
        }
        window.display();
    }
    return EXIT_SUCCESS;
}