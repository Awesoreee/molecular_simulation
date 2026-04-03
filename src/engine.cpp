#include <iostream>
#include "../SFML/include/SFML/Graphics.hpp"
#include <vector>
#include <cmath>
#include <random>
#include <unordered_map>
#include "../include/engine.hpp"

class atoms {
    std::random_device rd;
    std::mt19937 gen{rd()};
    sf::Vector2u zeroPos;
    sf::Vector2u borders;
    float sigmaNe = 2.74;
    float epsNe = 0.0031 * 1.602 * pow(10, -19);
    float massNe = 1.66;
    float cell_size = 2.5 * sigmaNe;

    std::vector<sf::Vector2f> veloc_all;
    std::vector<sf::Vector2f> forces_all;

    std::unordered_map<CellKey, std::vector<int>, CellKeyHash> spatial_table;

    public:
    std::vector<sf::Vector2f> coords_all;
    int amount = 0;

    atoms(int n, sf::Vector2u zero, sf::Vector2u bord){
        zeroPos = zero;
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
        build_spatial_hash();
        count_forces();
        int i;
        for (i=0;i<amount;i++){
            check_wall_collision(&(coords_all[i]), &(veloc_all[i]), forces_all[i], massNe, dt);
        }
        return 0;
    }

    void randomize(int max_velocity){
        int i;
        std::uniform_real_distribution<float> distx(float(zeroPos.x),float(zeroPos.x + borders.x));
        std::uniform_real_distribution<float> disty(float(zeroPos.y),float(zeroPos.y + borders.y));
        std::uniform_real_distribution<float> distv(0.0f,float(2*max_velocity));
        for (i = 0; i < amount; i++){
            coords_all[i] = {distx(gen), disty(gen)};
            veloc_all[i] = {distv(gen) - max_velocity, distv(gen) - max_velocity};
        }
    }
private:

    CellKey get_cell(sf::Vector2f pos) const {
        return {
            static_cast<int>(std::floor(pos.x / cell_size)),
            static_cast<int>(std::floor(pos.y / cell_size))
        };
    }

    void build_spatial_hash() {
        spatial_table.clear();
        for (int i = 0; i < amount; i++)
            spatial_table[get_cell(coords_all[i])].push_back(i);
    }

    std::vector<int> query_neighbors(sf::Vector2f pos) const {
        std::vector<int> result;
        CellKey center = get_cell(pos);
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                auto it = spatial_table.find({center.x + dx, center.y + dy});
                if (it != spatial_table.end())
                    for (int id : it->second)
                        result.push_back(id);
            }
        }
        return result;
    }

    sf::Vector2f count_force(sf::Vector2f coords1, sf::Vector2f coords2){
        sf::Vector2f diff = coords1 - coords2;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (distance < 1e-6) return {0, 0};
        float sr = sigmaNe / distance;
        float force = (48 * epsNe / sigmaNe) * (pow(sr, 13) - 0.5 * pow(sr, 7));
        return diff.normalized() * force;
    }

    int count_forces() {
        for (int i = 0; i < amount; i++) {
            forces_all[i] = {0.f, 0.f};
            for (int j : query_neighbors(coords_all[i])) {
                if (i != j)
                    forces_all[i] += count_force(coords_all[i], coords_all[j]);
            }
        }
        return 0;
    }

    int check_wall_collision(sf::Vector2f* coords, sf::Vector2f* veloc, sf::Vector2f force, float mass, float dt) {
        *coords += *veloc * dt + 0.5f * force * dt * dt / mass;
        *veloc += force * dt / mass;
        if (coords->x < zeroPos.x) {
            coords->x = 2.0f * zeroPos.x - coords->x;
            if (veloc->x < 0) veloc->x *= -1;
        } else if (coords->x > zeroPos.x + borders.x) {
            coords->x = 2.0f * (zeroPos.x + borders.x) - coords->x; 
            if (veloc->x > 0) veloc->x *= -1;
        }
        if (coords->y < zeroPos.y) {
            coords->y = 2.0f * zeroPos.y - coords->y; 
            if (veloc->y < 0) veloc->y *= -1;
        } else if (coords->y > zeroPos.y + borders.y) {
            coords->y = 2.0f * (zeroPos.y + borders.y) - coords->y;
            if (veloc->y > 0) veloc->y *= -1;
        }
        return 0;
    }
};