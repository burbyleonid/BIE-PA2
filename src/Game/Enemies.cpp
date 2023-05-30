
#include "Enemies.hpp"
#include "Respawn.hpp"


Enemy::Enemy(int health, int damage, const Respawn *resp) : m_health(health), m_damage(damage), m_resp(resp), pos(0)
{
}


std::pair<int, int> Enemy::getPosition() const {
  return m_resp->getPath()[pos];
}
