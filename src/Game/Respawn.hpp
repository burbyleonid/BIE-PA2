#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <sstream>

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

  bool isEmpty() const { return m_enemyCount <= 0; }

  Enemy getEnemy() {
    m_enemyCount--;
    return Enemy(10, 10, this);
  }

  const std::vector<std::pair<int, int>> &getPath() const { return m_path; }

  std::string toString() {
    std::stringstream out;
    out << m_enemyCount << " ";
    out << m_path.size() << " ";
    for (const auto &p : m_path) {
      out << p.first << ' ' << p.second << " ";
    }

    return out.str();
  }

  void fromString(std::string &str) {
    std::istringstream out(str);
    out >> m_enemyCount;
    size_t tmp;
    out >> tmp;
    m_path.clear();
    for (size_t i = 0; i < tmp; ++i) {
      std::pair<int, int> p;
      out >> p.first >> p.second;
      m_path.push_back(p);
    }

    return ;
  }



private:
  int m_enemyCount;
  std::vector<std::pair<int, int>> m_path;
};
