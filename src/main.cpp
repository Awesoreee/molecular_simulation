#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Audio.hpp"
#include "../SFML/include/SFML/Graphics/CircleShape.hpp"
#include "engine.cpp"
#include <vector>

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    int n = 3;
    atoms Atoms(n, {800, 600});
    Atoms.coords_all[0] = sf::Vector2f(200, 400);
    Atoms.coords_all[1] = sf::Vector2f(300, 400);
    Atoms.coords_all[2] = sf::Vector2f(200, 100);

    std::vector<sf::CircleShape> circs;
    circs.resize(n);

    for (int i = 0; i < n; i++){
        circs[i].setPosition(Atoms.coords_all[i])
    }
    

 
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
 
        window.clear();
 
        window.display();
    }
}