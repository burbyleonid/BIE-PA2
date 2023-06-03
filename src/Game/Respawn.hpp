#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <fstream>

#include "Enemies.hpp"

class Respawn {
public:
  Respawn(int enemyCount, std::pair<int, int> pos, std::pair<int, int> mainPos) : m_enemyCount(enemyCount)
  {
    m_path.push_back(pos);
    while (pos != mainPos) {
      if (abs(pos.first - mainPos.first) > abs(pos.second - mainPos.second)) {
        pos.first += (pos.first > mainPos.first ? -1 : 1);
      } else {
        pos.second += (pos.second > mainPos.second ? -1 : 1);
      }
      m_path.push_back(pos);
    }
  }

  Respawn(std::ifstream &f) {
    load(f);
  }
  void load(std::ifstream &f) {
    f >> m_enemyCount;
    size_t pathSize;
    f >> pathSize;
    for (size_t i = 0; i < pathSize; ++i) {
      int x, y;
      f >> x >> y;
      m_path.push_back({x, y});
    }

    size_t enemiesCount;

    f >> enemiesCount;
    for (size_t i = 0; i < enemiesCount; ++i) {
      m_enemies.push_back(Enemy(f));
    }
  }

  bool isEmpty() const { return m_enemyCount <= 0 && m_enemies.size() == 0; }

  void realiseEnemy() {
    m_enemyCount--;
    m_enemies.push_back(Enemy(10, 10));
  }

  std::pair<int, int> getCoordInPath(int pos) { return m_path[pos]; }

  const std::vector<std::pair<int, int>> &getPath() const { return m_path; }

  void save(std::ofstream &f) {
    f << m_enemyCount << " ";
    f << m_path.size() << " ";
    for (const auto &p : m_path) {
      f << p.first << ' ' << p.second << " ";
    }

    f << m_enemies.size() << std::endl;
    for (auto &enemy : m_enemies) {
      enemy.save(f);
    }
  }

  std::vector<Enemy> &getEnemies() { return m_enemies; }

private:
  int m_enemyCount;
  std::vector<std::pair<int, int>> m_path;
  std::vector<Enemy> m_enemies;
};
