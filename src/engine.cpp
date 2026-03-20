#include <iostream>
#include "../SFML/include/SFML/Graphics.hpp"
#include <vector>
#include <cmath>

class atoms {
    sf::Vector2f borders;
    std::vector<sf::Vector2f> coords_all;
    std::vector<sf::Vector2f> veloc_all;
    std::vector<sf::Vector2f> forces_all;
    int amount = 0;
    float sigmaNe = 2.67;
    float epsNe = 0.0031;
public:
    atoms(int n, float width, float height){
        borders = {width, height};
        coords_all.resize(n);
        veloc_all.resize(n);
        amount = n;
    }
    int add_atom(sf::Vector2f coords = sf::Vector2f(0.f, 0.f), sf::Vector2f veloc = sf::Vector2f(0.f, 0.f)){
        coords_all.push_back(coords);
        veloc_all.push_back(veloc);
        amount++;
    }
    int tick_forward(float dt){
        count_forces();
        int i;
        for (i=0;i++;i<amount){
            coords_all[i] += veloc_all[i] * dt + 0.5f * forces_all[i] * dt * dt;
        }
    }
private:
    sf::Vector2f count_force(sf::Vector2f coords1, sf::Vector2f coords2){
        sf::Vector2f diff = coords1 - coords2;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        float force = (48 * epsNe / sigmaNe) * (pow(sigmaNe / distance, 13) - 0.5 * pow(sigmaNe / distance, 7));
        return diff.normalized() * force;
    }
    int count_forces(){
        int i, j;
        for (i=0;i++;i<amount){
            for (j=0;j++;j<amount){
                if (i != j) {
                    forces_all[i] += count_force(coords_all[i], coords_all[j]);
                }
            }
        }
    }
    
};