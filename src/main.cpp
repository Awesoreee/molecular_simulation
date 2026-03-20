#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Audio.hpp"
#include "../SFML/include/SFML/Graphics/VertexArray.hpp"

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    sf::VertexArray points(sf::PrimitiveType::Points, 4);
    points[0].position = sf::Vector2f(200, 200);
    points[1].position = sf::Vector2f(300, 300);
    points[2].position = sf::Vector2f(400, 400);
    points[3].position = sf::Vector2f(500, 500);
 
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
 
        window.clear();
 
        window.draw(points);
 
        window.display();
    }
}