#include <iostream>
#include "../SFML/include/SFML/Graphics.hpp"
#include <vector>
#include <cmath>
#include <random>
#include <unordered_map>


class atoms {
    std::random_device rd;
    std::mt19937 gen{rd()};
    sf::Vector2u borders;
    float sigmaNe = 2.74;
    float epsNe = 0.0031 * 1.602 * pow(10, -19);
    float massNe = 1.66;
    float cell_size = 2.5 * sigmaNe;
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
        return 0;
    }

    int tick_forward(float dt){
        count_forces();
        int i;
        for (i=0;i<amount;i++){
            //coords_all[i] += veloc_all[i] * dt + 0.5f * forces_all[i] * dt * dt / massNe;
            //veloc_all[i] += forces_all[i] * dt / massNe;
            check_wall_collision(&(coords_all[i]), &(veloc_all[i]), forces_all[i], massNe, dt);
            //std::cout << i << "coords and veloc" << coords_all[i].x << coords_all[i].y << veloc_all[i].x << veloc_all[i].y << std::endl;
        }
        return 0;
    }

    void randomize(int max_velocity){
        int i;
        std::uniform_real_distribution<float> distx(0.0f,float(borders.x));
        std::uniform_real_distribution<float> disty(0.0f,float(borders.y));
        std::uniform_real_distribution<float> distv(0.0f,float(2*max_velocity));
        for (i = 0; i < amount; i++){
            coords_all[i] = {distx(gen), disty(gen)};
            veloc_all[i] = {distv(gen) - max_velocity, distv(gen) - max_velocity};
        }
    }
private:

    sf::Vector2f count_force(sf::Vector2f coords1, sf::Vector2f coords2){
        sf::Vector2f diff = coords1 - coords2;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (distance < 1e-6) return {0, 0};
        float sr = sigmaNe / distance;
        float force = (48 * epsNe / sigmaNe) * (pow(sr, 13) - 0.5 * pow(sr, 7));
        //std::cout << "Distance: " << distance << ", Force: " << force << std::endl;
        return diff.normalized() * force;
    }

    int count_forces(){
        int i, j;
        for (i=0;i<amount;i++){
            forces_all[i] = {0,0};
            for (j=0;j<amount;j++){
                if (i != j) {
                    //std::cout << i << j;
                    forces_all[i] += count_force(coords_all[i], coords_all[j]);
                }
            }
        }
        return 0;
    }

    int check_wall_collision(sf::Vector2f* coords, sf::Vector2f* veloc, sf::Vector2f force, float mass, float dt) {
        *coords += *veloc * dt + 0.5f * force * dt * dt / mass;
        *veloc += force * dt / mass;
        if (coords->x < 0) {
            coords->x = -coords->x;
            if (veloc->x < 0) veloc->x *= -1;
        } else if (coords->x > borders.x) {
            coords->x = 2.0f * borders.x - coords->x; 
            if (veloc->x > 0) veloc->x *= -1;
        }
        if (coords->y < 0) {
            coords->y = -coords->y; 
            if (veloc->y < 0) veloc->y *= -1;
        } else if (coords->y > borders.y) {
            coords->y = 2.0f * borders.y - coords->y;
            if (veloc->y > 0) veloc->y *= -1;
        }
        return 0;
    }

    
};