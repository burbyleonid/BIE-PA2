#ifndef Enemies_hpp
#define Enemies_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>


typedef enum {
  DefaultEnemyT = 0,
  ProbEnemyT
} EnemyType;


// Base class: Enemy
class Enemy {
public:
  Enemy(int health, int damage) : m_health(health), m_damage(damage), m_pos(0) { }
  Enemy(std::ifstream &f) { load(f);}

  int getPosition() const { return m_pos; }

  virtual bool takeDamage(int damage) {
    m_health -= damage;
    std::cout << "The enemy takes " << damage << " damage. Current HP is " << m_health <<"\n";
    if (m_health <= 0) {
      std::cout << "The enemy has been defeated.\n";
      return false;
    }
    
    return true;
  }

  void save(std::ofstream &f) const {
    f << m_id << std::endl;
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

  EnemyType getId() const { return m_id; }

  int getDamage() const { return m_damage; }

protected:
  // Attributes
  int m_health;
  int m_damage;

  // Position
  int m_pos;
  EnemyType m_id = DefaultEnemyT;
};


class ProbEnemy : public Enemy {
public:
  ProbEnemy(int health, int damage) : Enemy(health, damage) { m_id = ProbEnemyT; }
  ProbEnemy(std::ifstream &f) : Enemy(f) { m_id = ProbEnemyT; }

  bool takeDamage(int damage) override {
    if (rand() % 100 < 50) {
      m_health -= damage;
      std::cout << "The enemy takes " << damage << " damage. Current HP is " << m_health <<"\n";
      if (m_health <= 0) {
        std::cout << "The enemy has been defeated.\n";
        return false;
      }
    } else {
      std::cout << "The enemy dodged the attack. \n";
    }

    return true;
  }

  int getDamage() const { return m_damage; }
};

#endif
