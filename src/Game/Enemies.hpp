#ifndef Enemies_hpp
#define Enemies_hpp

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

class Respawn;


// Base class: Enemy
class Enemy {
public:
  Enemy(int health, int damage, const Respawn *resp);

  void attack() {
    std::cout << "The enemy attacks and deals " << m_damage << " damage.\n";
  }

  bool takeDamage(int damage) {
    m_health -= damage;
    std::cout << "The enemy takes " << damage << " damage.\n";
    if (m_health <= 0) {
      std::cout << "The enemy has been defeated.\n";
      return false;
    }
    
    return true;
  }

  // A* algorithm implementation
  bool move(std::pair<int, int> mainBuild) {
    pos++;
    if (getPosition() == mainBuild) {
      return true;
    }
    return false;
  }

  int getDamage() const { return m_damage; }

  std::pair<int, int> getPosition() const;


protected:

  const Respawn *m_resp;

  // Attributes
  int m_health;
  int m_damage;

  // Position
  int pos;
};

#endif
