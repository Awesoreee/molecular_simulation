#include <iostream>
#include "../SFML/include/SFML/Graphics.hpp"
#include <vector>
#include <cmath>

class atoms {
    sf::Vector2u borders;
    float sigmaNe = 2.67;
    float epsNe = 1;
    std::vector<sf::Vector2f> veloc_all;
    std::vector<sf::Vector2f> forces_all;

    public:
    std::vector<sf::Vector2f> coords_all;
    int amount = 0;

    atoms(int n, sf::Vector2u bord){
        borders = bord;
        coords_all.resize(n);
        veloc_all.resize(n);
        forces_all.resize(n);
        amount = n;
    }
    int add_atom(sf::Vector2f coords = sf::Vector2f(0.f, 0.f), sf::Vector2f veloc = sf::Vector2f(0.f, 0.f)){
        coords_all.push_back(coords);
        veloc_all.push_back(veloc);
        forces_all.push_back({1,1});
        amount++;
    }
    int tick_forward(float dt){
        count_forces();
        int i;
        for (i=0;i<amount;i++){
            coords_all[i] += veloc_all[i] * dt + 0.5f * forces_all[i] * dt * dt;
            veloc_all[i] += forces_all[i] * dt;
            std::cout << i << "coords and veloc" << coords_all[i].x << coords_all[i].y << veloc_all[i].x << veloc_all[i].y << std::endl;
        }
        return 0;
    }
private:
    sf::Vector2f count_force(sf::Vector2f coords1, sf::Vector2f coords2){
        sf::Vector2f diff = coords1 - coords2;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        float force = (48 * epsNe / sigmaNe) * (pow(sigmaNe / distance, 13) - 0.5 * pow(sigmaNe / distance, 7));
        std::cout << "Distance: " << distance << ", Force: " << force << std::endl;
        return diff.normalized() * force;
    }
    int count_forces(){
        int i, j;
        for (i=0;i<amount;i++){
            forces_all[i] = {0,0};
            for (j=0;j<amount;j++){
                if (i != j) {
                    std::cout << i << j;
                    forces_all[i] += count_force(coords_all[i], coords_all[j]);
                }
            }
        }
        return 0;
    }
    
};