#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Audio.hpp"
#include "../SFML/include/SFML/Graphics/CircleShape.hpp"
#include "engine.cpp"
#include <vector>
#include <iostream>

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    int n = 3;
    atoms Atoms(n, {800, 600});
    Atoms.coords_all[0] = sf::Vector2f(100, 400);
    Atoms.coords_all[1] = sf::Vector2f(300, 400);
    Atoms.coords_all[2] = sf::Vector2f(110, 400);

    std::vector<sf::CircleShape> circs;
    circs.resize(n);

    for (int i = 0; i < n; i++){
        circs[i].setPosition(Atoms.coords_all[i]);
        circs[i].setRadius(5);
    }
    

 
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        for(int j = 0; j < 100000; j++){
            window.clear();

            Atoms.tick_forward(0.1);

            for(int i = 0; i < n; i++){
                circs[i].setPosition(Atoms.coords_all[i]);
                window.draw(circs[i]);

                std::cout << Atoms.coords_all[i].x << "|";
            }
            std::cout << std::endl;

            window.display();
        }
    }
}