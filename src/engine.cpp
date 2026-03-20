#include <iostream>
#include "../SFML/include/SFML/Graphics.hpp"
#include <vector>

class atoms {
    std::vector<sf::Vector2f> coords;
    sf::Vector2f borders;
public:
    atoms(int n, float x, float y){
        borders = {x, y};
    }
    int eng_main(){
        coords.push_back({0,0});
        coords[1].x = 50.f;
        coords[1].y = 100.f;
    }
};