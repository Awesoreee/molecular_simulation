#pragma once

#include "../SFML/include/SFML/System/Vector2.hpp"
#include <vector>
#include <random>
#include <unordered_map>

struct CellKey {
    int x, y;
    bool operator==(const CellKey& o) const { return x == o.x && y == o.y; }
};

struct CellKeyHash {
    size_t operator()(const CellKey& c) const;
};

/**
 * @brief Main class to work with atoms
 * 
 */
class atoms {
    std::random_device rd;
    std::mt19937 gen{rd()};
    sf::Vector2u zeroPos;
    sf::Vector2u borders;
    float sigmaNe;
    float epsNe;
    float massNe;
    float cell_size;

    std::vector<sf::Vector2f> veloc_all;
    std::vector<sf::Vector2f> forces_all;

    std::unordered_map<CellKey, std::vector<int>, CellKeyHash> spatial_table;

    public:
    /**
     * @brief list with all atoms coordinates
     * 
     */
    std::vector<sf::Vector2f> coords_all;

    /**
     * @brief amount of existing atoms
     * 
     */
    int amount;

    /**
     * @brief Construct a new atoms object
     * 
     * @param n amount of start atoms
     * @param zero coordinats of upper left corner of working space
     * @param bord size of borders
     */
    atoms(int n, sf::Vector2u zero, sf::Vector2u bord);

    /**
     * @brief add new atom
     * 
     * @param coords coordinats of new atom (by default (0,0))
     * @param veloc velocity of new atom (by default (0,0))
     * @return int (0)
     */
    int add_atom(sf::Vector2f coords = sf::Vector2f(0.f, 0.f), sf::Vector2f veloc = sf::Vector2f(0.f, 0.f));

    /**
     * @brief run one tick forward
     * 
     * @param dt time of tick
     * @return int (0)
     */
    int tick_forward(float dt);

    /**
     * @brief randomize all existing atoms within the borders
     * 
     * @param max_velocity maximum random velocity of randomized atoms
     */
    void randomize(int max_velocity);
private:

    CellKey get_cell(sf::Vector2f pos);

    void build_spatial_hash();

    std::vector<int> query_neighbors(sf::Vector2f pos);

    sf::Vector2f count_force(sf::Vector2f coords1, sf::Vector2f coords2);

    int count_forces();

    int check_wall_collision(sf::Vector2f* coords, sf::Vector2f* veloc, sf::Vector2f force, float mass, float dt);
};