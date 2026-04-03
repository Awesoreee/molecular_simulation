#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Audio.hpp"
#include "../SFML/include/SFML/Graphics/CircleShape.hpp"
#include "../SFML/include/SFML/Graphics/RectangleShape.hpp"
#include "../SFML/include/SFML/System/Time.hpp"
#include "../SFML/include/SFML/System/Clock.hpp"
#include "../TGUI/include/TGUI/Backend/SFML-Graphics.hpp"
#include "../TGUI/include/TGUI/TGUI.hpp"
#include "../include/engine.hpp"
#include <vector>
#include <iostream>
#include <string.h>

void addButton(tgui::Gui& gui, std::string Name, bool& status, sf::Vector2f coords, sf::Vector2f size)
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

sf::Time changeTimePerSec(int tick){
    return sf::seconds(1.f / tick);
}

void addTimeSlider(tgui::Gui& gui, int& tick, sf::Time& TimePerSec, sf::Vector2f coords, sf::Vector2f size, int max, int min)
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
    sf::Vector2u quality = {1920, 1080};
    sf::RenderWindow window(sf::VideoMode(quality), "SFML window");
    float windowWidth = static_cast<float>(window.getSize().x);
    float windowHeight = static_cast<float>(window.getSize().y);
    tgui::Gui gui{window}; 

    bool AddMode = false;
    addButton(gui, "Add atom", AddMode, {windowWidth - 80.f, windowHeight - 40.f}, {70.f, 30.f});

    bool finish = false;
    addButton(gui, "Exit", finish, {10.f, windowHeight - 40.f}, {70.f, 30.f});

    bool isFullscreen = false;
    addButton(gui, "Full", isFullscreen, {10.f, windowHeight - 70.f}, {70.f, 30.f});

    // Time initialization
    bool timeIsStop = false;
    addButton(gui, "Pause", timeIsStop, {windowWidth - 125.f, 10.f}, {70.f, 30.f});
    
    int tick = 60;
    sf::Clock clock;
    sf::Time TimeSinceLastUpgrade = sf::Time::Zero;
    sf::Time TimePerSec = changeTimePerSec(tick);

    addTimeSlider(gui, tick, TimePerSec, {windowWidth - 150.f, 50.f}, {120, 10}, 300, 60);

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
    Atoms.randomize(5);

    std::vector<sf::CircleShape> circs;
    circs.resize(n);

    for (int i = 0; i < n; i++)
    {
        circs[i].setPosition(Atoms.coords_all[i]);
        circs[i].setRadius(3);
    }

    sf::RectangleShape rectangle;
    sf::Vector2f sizeOfRect = {sizeOfBorder.x + 5.f, sizeOfBorder.y + 5.f};
    rectangle.setSize(sizeOfRect);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(5);
    rectangle.setPosition ((sf::Vector2f)coordOfBorder);
    rectangle.setFillColor(sf::Color::Transparent);
    

    int frameCount = 0;
    bool lastState = isFullscreen;

    // Draw window
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            gui.handleEvent(*event);
            if (event->is<sf::Event::Closed>() || finish)
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
            if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::F11)
                {
                    isFullscreen = !isFullscreen;

                    if(isFullscreen)
                    {
                        window.create(sf::VideoMode::getDesktopMode(), "Molecular Simulation", sf::Style::Default, sf::State::Fullscreen);
                    }
                    else
                    {
                        window.create(sf::VideoMode(quality), "Molecular Simulation", sf::Style::Default, sf::State::Windowed);
                    }

                    lastState = isFullscreen;
                }
            }
            if(isFullscreen != lastState)
            {
                if(isFullscreen)
                {
                    window.create(sf::VideoMode::getDesktopMode(), "Molecular Simulation", sf::Style::Default, sf::State::Fullscreen);
                }
                else
                {
                    window.create(sf::VideoMode(quality), "Molecular Simulation", sf::Style::Default, sf::State::Windowed);
                }

                lastState = isFullscreen;
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
        window.draw(rectangle);

        gui.draw(); 
        window.display();
    }
}