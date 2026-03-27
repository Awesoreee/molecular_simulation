#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Audio.hpp"
#include "../SFML/include/SFML/Graphics/CircleShape.hpp"
#include "../SFML/include/SFML/System/Time.hpp"
#include "../SFML/include/SFML/System/Clock.hpp"
#include "../TGUI/include/TGUI/Backend/SFML-Graphics.hpp"
#include "../TGUI/include/TGUI/TGUI.hpp"
#include "engine.cpp"
#include <vector>
#include <iostream>
#include <string.h>

void addTimeButton(tgui::Gui& gui, std::string Name, bool& time, sf::Vector2u coords, sf::Vector2u size){
    auto button = tgui::Button::create(Name);
    auto [x, y] = coords;
    button->setPosition(x, y);
    auto [x_size, y_size] = size;
    button->setSize(x_size, y_size);

    button->onPress([&time](){
        time = !time;
    });

    gui.add(button);
}

sf::Time changeTimePerSec(int tick){
    return sf::seconds(1.f / tick);
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({1980, 1080}), "SFML window");
    tgui::Gui gui{window}; 

    // Time initialization
    bool timeIsStop = false;
    addTimeButton(gui, "Pause", timeIsStop, {700, 500}, {70, 30});
    
    int tick = 60;
    sf::Clock clock;
    sf::Time TimeSinceLastUpgrade = sf::Time::Zero;
    sf::Time TimePerSec = changeTimePerSec(tick);

    auto slider = tgui::Slider::create();
    slider->setPosition(700, 600);
    slider->setSize(200, 20);
    slider->setMaximum(120);
    slider->setMinimum(10);
    slider->setValue(60);

    slider->onValueChange([&tick, &TimePerSec](int new_tick){
        tick = new_tick;
        TimePerSec = changeTimePerSec(tick);
    });

    gui.add(slider);

    // Add atoms
    int n = 7;
    atoms Atoms(n, {1280, 720});
    Atoms.randomize(5);

    std::vector<sf::CircleShape> circs;
    circs.resize(n);

    for (int i = 0; i < n; i++){
        circs[i].setPosition(Atoms.coords_all[i]);
        circs[i].setRadius(3);
    }
    
    // Draw window
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            gui.handleEvent(*event);
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        if (timeIsStop != false){
            TimeSinceLastUpgrade += clock.restart();

            while(TimeSinceLastUpgrade >= TimePerSec){
                TimeSinceLastUpgrade -= TimePerSec;
                Atoms.tick_forward(0.1);
                for(int i = 0; i < n; i++){
                    circs[i].setPosition(Atoms.coords_all[i]);
                }
            }
        }
        else{
            clock.restart();
        }

        window.clear();

        for (int i = 0; i < n; i++){
            window.draw(circs[i]);
        }

        gui.draw(); 
        window.display();
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