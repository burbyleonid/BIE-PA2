#include "Game.hpp"
#include <fstream>

// all resources are 10 by default
Game::Game(int lvl) : m_resources({100, 100, 100}){
  for (size_t i = 0; i < m_width; ++i) {
    m_map.push_back(std::vector<shared_ptr<Field>>());
    for (size_t j = 0; j < m_height; ++j) {
      // type = -1, wokers count = 0
      m_map[i].emplace_back(make_shared<Field>(-1));
    }
  }

  initCost();

  m_buildsDamage = { 1, 2, 4, 8};

  // town hall
  m_map[0][m_height - 1]->m_type = 5;
  m_mainBuild = { 0, m_height - 1};
  m_mainHealth = 30;

  switch (lvl) {
    case 1: {
      m_respawns.push_back(Respawn(3, {m_width - 1, 0}, m_mainBuild));
      break;
    }
    case 2: {
      m_respawns.push_back(Respawn(5, {0, 0}, m_mainBuild));
      m_respawns.push_back(Respawn(3, {m_width - 1, m_height - 1}, m_mainBuild));
      break;
    }

    default:
      break;
  }


  for (auto &resp : m_respawns) {
    const auto &path = resp.getPath();
    for (int i = 0; i < path.size() - 1; ++i) {
      m_map[path[i].first][path[i].second]->m_type = -2;
    }
  }
}

// adding building on map with type and coords
void Game::addBuild(int buildType, int i, int j) {
  m_builds[buildType][{i, j}] = false;
  m_map[i][j]->m_type = buildType;
  for (int i = 0; i < m_resources.size(); ++i) {
    m_resources[i] -= m_buildsCost[buildType][i];
  }
}

bool Game::nextStep() {
  m_daysCount++;

  std::vector<int> del;

  for (auto &resp : m_respawns) {
    std::vector<int> del;
    auto &enemies = resp.getEnemies();
    for (int i = 0; i < enemies.size(); ++i) {
      auto &enemy = enemies[i];
      auto tmp = resp.getCoordInPath(enemy.getPosition());
      m_map[tmp.first][tmp.second]->m_type = -2;

      int pos = enemy.move();

      if (resp.getCoordInPath(pos) == m_mainBuild) {
        m_mainHealth -= enemy.getDamage();
        std:cout<<"Town hall got damaged, " << m_mainHealth << " HP left." << std::endl;
        del.push_back(i);
      }
    }

    for (int i = (int)(del.size() - 1); i >= 0; i--) {
      enemies.erase(enemies.begin() + del[i]);
    }
  }

  if (m_mainHealth <= 0) {
    return false;
  }

  for (auto &resp : m_respawns) {
    auto &enemies = resp.getEnemies();
    for (int i = 0; i < enemies.size(); ++i) {
      auto &enemy = enemies[i];
      auto tmp = resp.getCoordInPath(enemy.getPosition());
      m_map[tmp.first][tmp.second]->m_type = -3;
    }
  }

  for (auto &resp : m_respawns) {
    if (!resp.isEmpty()) {
      auto &enemies = resp.getEnemies();
      resp.realiseEnemy();
      auto tmp = resp.getCoordInPath(enemies.rbegin()->getPosition());
      m_map[tmp.first][tmp.second]->m_type = -3;
    }
  }

  for (auto &m : m_builds) {
    for (auto &p : m) {
      p.second = false;
    }
  }

  return true;
}

// uses osome "formula" to calculate final score
int Game::getScore() const {
  int res = 0;
  for (auto r : m_resources) {
    res += r;
  }
  res += m_daysCount;

  res += m_enemyScore;

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
  f >> m_mainBuild.first >> m_mainBuild.second;
  f >> m_mainHealth;

  for (size_t i = 0; i < m_width; ++i) {
    for (size_t j = 0; j < m_height; ++j) {
      int type;
      f >> type;
      m_map[i][j] = make_shared<Field>(type);
    }
  }

  for (int type = 0; type < 4; ++type) {
    // cout << type << endl;
    size_t size;
    f >> size;
    // cout << size << endl;
    for (size_t i = 0; i < size; ++i) {
      int x, y;
      bool status;
      f >> x >> y >> status;
      m_builds[type][{x, y}] = status;

      // cout << x << ' ' << y << ' ' << status << std::endl;
    }
  }

  for (size_t i = 0; i < m_resources.size(); ++i) {
    f >> m_resources[i];
  }

  f >> m_enemyScore;

  size_t respCount;
  f >> respCount;

  for (size_t i = 0; i < respCount; ++i) {
    m_respawns.push_back(Respawn(f));
  }

  return true;
}

void Game::save(const std::string &fileName) {
  ofstream f;
  f.open(fileName, ios::out);

  f << m_daysCount << std::endl;
  f << m_mainBuild.first << " " << m_mainBuild.second << std::endl;

  f << m_mainHealth << std::endl;

  for (size_t i = 0; i < m_width; ++i) {
    for (size_t j = 0; j < m_height; ++j) {
      f << m_map[i][j]->m_type << " ";
    }
    f << std::endl;
  }

  for (int type = 0; type < 4; ++type) {
    // cout << type << endl;
    f << m_builds[type].size() << endl;
    // cout << "Size = " << m_builds[type].size() << endl;
    for (auto &p : m_builds[type]) {
      f << p.first.first << ' ' << p.first.second << ' ' << p.second << std::endl;

      // cout << "(x, y, status) = " << p.first.first << ' ' << p.first.second << ' ' << p.second << std::endl;
    }
  }

  for (size_t i = 0; i < m_resources.size(); ++i) {
    f << m_resources[i] << " ";
  }
  f << std::endl;

  f << m_enemyScore << std::endl;

  f << m_respawns.size() << std::endl;
  for (auto &resp : m_respawns) {
    resp.save(f);
  }
}


