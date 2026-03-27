#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Audio.hpp"
#include "../SFML/include/SFML/Graphics/CircleShape.hpp"
#include "../SFML/include/SFML/System/Time.hpp"
#include "../SFML/include/SFML/System/Clock.hpp"
#include "engine.cpp"
#include <vector>
#include <iostream>

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    sf::Clock clock;
    sf::Time TimeSinceLastUpgrade = sf::Time::Zero;
    sf::Time TimePerSec = sf::seconds(1.f / 60.f);

    int n = 20;
    atoms Atoms(n, {800, 600});
    Atoms.randomize(1);

    std::vector<sf::CircleShape> circs;
    circs.resize(n);

    for (int i = 0; i < n; i++){
        circs[i].setPosition(Atoms.coords_all[i]);
        circs[i].setRadius(3);
    }
    

 
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        

        TimeSinceLastUpgrade += clock.restart();

        while(TimeSinceLastUpgrade >= TimePerSec){
            window.clear();

            TimeSinceLastUpgrade -= TimePerSec;
            Atoms.tick_forward(0.1);
            for(int i = 0; i < n; i++){
                window.draw(circs[i]);
                circs[i].setPosition(Atoms.coords_all[i]);
            }
            window.display();
        }

        /*
        for(int i = 0; i < n; i++){
            circs[i].setPosition(Atoms.coords_all[i]);
            window.draw(circs[i]);

            std::cout << Atoms.coords_all[i].x << "|";
        }
        std::cout << std::endl;
        */
    }
}