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

void addButton(tgui::Gui& gui, std::string Name, bool& status, sf::Vector2u coords, sf::Vector2u size)
{
    auto button = tgui::Button::create(Name);
    auto [x, y] = coords;
    button->setPosition(x, y);
    auto [x_size, y_size] = size;
    button->setSize(x_size, y_size);

    button->onPress([&status](){
        status = !status;
    });

    gui.add(button);
}

void addTimeButton(tgui::Gui& gui, std::string Name, bool& time, sf::Vector2u coords, sf::Vector2u size)
{
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

void addTimeSlider(tgui::Gui& gui, int& tick, sf::Time& TimePerSec, sf::Vector2u coords, sf::Vector2u size, int max, int min)
{
    auto slider = tgui::Slider::create();
    auto [x, y] = coords;
    slider->setPosition(x, y);
    auto [x_size, y_size] = size;
    slider->setSize(x_size, y_size);
    slider->setMaximum(max);
    slider->setMinimum(min);
    slider->setValue(min);

    slider->onValueChange([&tick, &TimePerSec](int new_tick)
    {
        tick = new_tick;
        TimePerSec = changeTimePerSec(tick);
    });

    gui.add(slider);
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "SFML window");
    tgui::Gui gui{window}; 

    // Time initialization
    bool timeIsStop = false;
    addButton(gui, "Pause", timeIsStop, {700, 500}, {70, 30});

    bool AddMode = false;
    addButton(gui, "Add atom", AddMode, {700, 700}, {70, 30});
    
    int tick = 60;
    sf::Clock clock;
    sf::Time TimeSinceLastUpgrade = sf::Time::Zero;
    sf::Time TimePerSec = changeTimePerSec(tick);

    addTimeSlider(gui, tick, TimePerSec, {600, 500}, {120, 10}, 300, 60);

    sf::Font font;
    font.openFromFile("../data/ObelixProB-cyr.ttf");

    sf::Text fpsText(font);
    fpsText.setCharacterSize(18);
    fpsText.setPosition({10.f, 10.f});

    sf::Time fpsTimer = sf::Time::Zero;

    // Add atoms
    int n = 250;
    sf::Vector2u coordOfBorder = {640, 360};
    sf::Vector2u sizeOfBorder = {640, 360};
    atoms Atoms(n, coordOfBorder, sizeOfBorder);
    Atoms.randomize(1);

    std::vector<sf::CircleShape> circs;
    circs.resize(n);

    for (int i = 0; i < n; i++)
    {
        circs[i].setPosition(Atoms.coords_all[i]);
        circs[i].setRadius(3);
    }
    

    int frameCount = 0; 
    // Draw window
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            gui.handleEvent(*event);
            if (event->is<sf::Event::Closed>())
                window.close();
            if (AddMode)
            {
                if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if(mouseClick->button == sf::Mouse::Button::Left)
                    {
                        if(
                            (sf::Mouse::getPosition(window).x > coordOfBorder.x) &&
                            (sf::Mouse::getPosition(window).x < coordOfBorder.x + sizeOfBorder.x) &&
                            (sf::Mouse::getPosition(window).y > coordOfBorder.y) &&
                            (sf::Mouse::getPosition(window).y < coordOfBorder.y + sizeOfBorder.y)
                        )
                        {
                            Atoms.add_atom((sf::Vector2f)sf::Mouse::getPosition(window));
                            n++;
                            circs.resize(n);
                            circs[n-1].setPosition(Atoms.coords_all[n-1]);
                            circs[n-1].setRadius(3);
                        }
                    }
                }
            }
        }
        
        sf::Time deltaTime = clock.restart();

        fpsTimer += deltaTime;
        frameCount++;

        if (fpsTimer >= sf::seconds(1.f))
        {
            float fps = frameCount / fpsTimer.asSeconds();
            fpsText.setString("Fps: " + std::to_string(static_cast<int>(fps)));

            fpsTimer = sf::Time::Zero;
            frameCount = 0;
        }

        if (timeIsStop != false){
            TimeSinceLastUpgrade += deltaTime;

            while(TimeSinceLastUpgrade >= TimePerSec)
            {
                TimeSinceLastUpgrade -= TimePerSec;
                Atoms.tick_forward(0.1);
                for(int i = 0; i < n; i++)
                {
                    circs[i].setPosition(Atoms.coords_all[i]);
                }
            }
        }

        window.clear();

        for (int i = 0; i < n; i++)
        {
            window.draw(circs[i]);
        }

        window.draw(fpsText);

        gui.draw(); 
        window.display();
    }
}