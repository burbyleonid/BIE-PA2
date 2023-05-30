#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <memory>
#include "Field.hpp"
#include "Respawn.hpp"

using namespace std;

class Game{
public:

  Game(int lvl);
  ~Game() = default;

  // map is a matrix of Fields, used for loading game
  vector<vector<shared_ptr<Field>>> &getMap() { return m_map; }

  bool isWin() const {
    if (m_enemies.size()) {
      return false;
    }
    for (const auto &resp : m_respawns) {
      if (!resp.isEmpty()) {
        return false;
      }
    }
    return true;
  }

  // 3 - Gold, steel, wood
  const array<int, 3> &getResources() const { return m_resources; }

  // days count, resourses
  bool nextStep();

  // type of building, position a coordinates
  void addBuild(int type, int i, int j);

  bool towerToEnemy(std::pair<int, int> towerPos, std::pair<int, int> enemyPos) {
    int towerDamage = m_buildsDamage[m_map[towerPos.first][towerPos.second]->m_type];

    for (int i = 0; i < m_enemies.size(); ++i) {
      auto &enemy = m_enemies[i];
      if (enemy.getPosition() == enemyPos) {
        if (!enemy.takeDamage(towerDamage)) {
          m_enemyScore += 10;
          m_enemies.erase(m_enemies.begin() + i);

          return true;
        }
        return false;
      }
    }

    return false;
  }

  // if meat >= 0, game continues
  // bool isRunning() { return m_resources[3] >= 0; }

  // + 1 worker
  // void buyWorker();

  // needed for scoring system
  int getDaysCount() const { return m_daysCount; }

  // save game with name $(fileName) in dir /src/SavedGmes
  void save(const std::string &fileName);

  pair<int, int> getMainBuildCord() const { return m_mainBuild; }

  // coordinates  to type
  int getCellType(int i, int j) const { return m_map[i][j]->m_type; }

  // if feild not empty returns work counter as string
  // string getCellInfo(int i, int j) const {
  //     return (m_map[i][j]->m_type != -1 &&  m_map[i][j]->m_workerCount != 0 ? std::to_string(m_map[i][j]->m_workerCount) : "") ;
  // }

  // Gold, Steel, Wood, Meat
  string getResourcesText() const { return ( "G:"+std::to_string(m_resources[0])+" "
                                            +"S:"+std::to_string(m_resources[1])+" "
                                            +"W:"+std::to_string(m_resources[2])+" "
                                            //   +"M:"+std::to_string(m_resources[3])
                                            );}

  // if player has enough resources he can buy building
  bool checkCost(int buildType) {
    for (int i = 0; i < m_resources.size(); ++i) {
      if (m_resources[i] < m_buildsCost[buildType][i]) {
        return false;
      }
    }
    return true;
  }

  // when game is over, get players score
  int getScore() const;

  // working with coordinstes
  size_t getWidth() const { return m_width; }
  size_t getHeight() const { return m_height; }

  // load game with name $(fileName) from dir /src/SavedGmes
  bool load(const std::string &fileName);

private:
  // buildings and amount of workes
  void initCost();
private:
  const size_t m_height = 10;
  const size_t m_width = 10;

  int m_daysCount = 1;

  int m_enemyScore = 0;

  const array<int, 3> m_resourcesAdd = { 1, 1, 1};

  pair<int, int> m_mainBuild;
  int m_mainHealth;

  vector<vector<shared_ptr<Field>>> m_map;
  array<int, 3> m_resources;
  array<set<pair<int, int>>, 4> m_builds;

  array<array<int, 4>, 4> m_buildsCost;
  array<int, 4> m_buildsDamage;

  std::vector<Respawn> m_respawns;
  std::vector<Enemy> m_enemies;

  // int m_workersCount = 0;

};

#endif /* Game_hpp */
