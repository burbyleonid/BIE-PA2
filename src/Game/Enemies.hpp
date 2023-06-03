#ifndef Enemies_hpp
#define Enemies_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>


// Base class: Enemy
class Enemy {
public:
  Enemy(int health, int damage) : m_health(health), m_damage(damage), m_pos(0) { }
  Enemy(std::ifstream &f) { load(f); }

  int getPosition() const { return m_pos; }

  bool takeDamage(int damage) {
    m_health -= damage;
    std::cout << "The enemy takes " << damage << " damage. His HP now is " << m_health << std::endl;
    if (m_health <= 0) {
      std::cout << "The enemy has been defeated.\n";
      return false;
    }
    
    return true;
  }

  void save(std::ofstream &f) const {
    f << m_health << std::endl;
    f << m_damage << std::endl;
    f << m_pos << std::endl;
  }
  
  void load(std::ifstream &f) {
    f >> m_health;
    f >> m_damage;
    f >> m_pos;
  }

  int move() { return ++m_pos; }

  int getDamage() const { return m_damage; }

protected:
  // Attributes
  int m_health;
  int m_damage;

  // Position
  int m_pos;
};

#endif
