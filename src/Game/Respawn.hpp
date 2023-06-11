#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <fstream>

#include "Enemies.hpp"

class Respawn {
public:
  Respawn() = default;
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

  ~Respawn() {
    for (int i = 0; i < m_enemies.size(); ++i) {
      delete m_enemies[i];
    }
    m_enemies.clear();
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
      int id;
      f >> id;
      switch (id) {
        case DefaultEnemyT: {
          m_enemies.push_back(new Enemy(f));
          break;
        }
        case ProbEnemyT: {
          m_enemies.push_back(new ProbEnemy(f));
          break;
        }

        default:
          break;
      }
    }
  }

  bool isEmpty() const { return m_enemyCount <= 0 && m_enemies.size() == 0; }

  bool realiseEnemy() {
    if (m_enemyCount > 0) {
      m_enemyCount--;
      if (m_enemyCount & 1)
        m_enemies.push_back(new Enemy(10, 10));
      else
        m_enemies.push_back(new ProbEnemy(10, 10));
      return true;
    }
    return false;
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
      enemy->save(f);
    }
  }

  std::vector<Enemy *> &getEnemies() { return m_enemies; }

private:
  int m_enemyCount = 0;
  std::vector<std::pair<int, int>> m_path;
  std::vector<Enemy *> m_enemies;
};
