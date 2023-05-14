#include "Game.hpp"
#include <fstream>

// all resources are 10 by default
Game::Game() : m_resources({100, 100, 100}) {
    for (size_t i = 0; i < m_width; ++i) {
        m_map.push_back(std::vector<shared_ptr<Field>>());
        for (size_t j = 0; j < m_height; ++j) {
            // type = -1, wokers count = 0
            m_map[i].emplace_back(make_shared<Field>(-1));
        }
    }

    initCost();

    // town hall
    m_map[m_width / 2][m_height / 2]->m_type = 5;
    m_mainBuild = { m_width / 2, m_height / 2 };
}

// adding building on map with type and coords
void Game::addBuild(int buildType, int i, int j) {
    m_builds[buildType].insert({i, j});
    m_map[i][j]->m_type = buildType;
    for (int i = 0; i < m_resources.size(); ++i) {
        m_resources[i] -= m_buildsCost[buildType][i];
    }
}


pair<int, int> Game::nextStep() {
    m_daysCount++;
    return {-1, -1};
    
}

// uses osome "formula" to calculate final score
int Game::getScore() const {
    int res = 0;
    for (auto r : m_resources) {
        res += r;
    }
    res += m_daysCount;

    return max(res, 0);
}


// costs of buildings
void Game::initCost() {
    m_buildsCost[0] = {1,2,3};
    m_buildsCost[1] = {1,2,3};
    m_buildsCost[2] = {1,2,3};
    m_buildsCost[3] = {1,2,3};
}

bool Game::load(const std::string &fileName) {
    ifstream f;
    f.open(fileName, ios::in);
    f >> m_daysCount;
    // f >> m_workersCount;
    f >> m_mainBuild.first >> m_mainBuild.second;


    for (size_t i = 0; i < m_width; ++i) {
        for (size_t j = 0; j < m_height; ++j) {
            int type;
            f >> type;
            m_map[i][j] = make_shared<Field>(type);
            if (type >= 0 && type <= 3) {
                m_builds[type].insert({i, j});
            }
        }
    }

    for (size_t i = 0; i < m_resources.size(); ++i) {
        f >> m_resources[i];
    }

    return true;
}

void Game::save(const std::string &fileName) {
    ofstream f;
    f.open(fileName, ios::out);

    f << m_daysCount << std::endl;
    // f << m_workersCount << std::endl;
    f << m_mainBuild.first << " " << m_mainBuild.second << std::endl;

    for (size_t i = 0; i < m_width; ++i) {
        for (size_t j = 0; j < m_height; ++j) {
            f << m_map[i][j]->m_type << " ";
        }
        f << std::endl;
    }

    for (size_t i = 0; i < m_resources.size(); ++i) {
        f << m_resources[i] << " ";
    }
    f << std::endl;
}


