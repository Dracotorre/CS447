// Entity.cpp
/*
   David Shrock
   CS447 - PSU
   2004.07.05

   implementation of Entity

   Base class definition for an entity (object) in the world that can be
   interacted with, such as a wall, door, chair, vase, person, etc. All
   entities in the world should be derived from this class.
*/
#include "Entity.h"
#include "intersections.h"           // for collision testing

// default constructor
Entity::Entity()
{
  m_initialized = 0;
  m_yaw = 0.0f;
  m_pitch = 0.0f;
}

// copy constructor
Entity::Entity(const Entity &source)
{
  m_yaw = source.m_yaw;
  m_pitch = source.m_pitch;
  m_position = source.m_position;
  m_velocity = source.m_velocity;
  //acceleration = source.acceleration;
  m_initialized = source.m_initialized;
  m_bbMax = source.m_bbMax;
  m_bbMin = source.m_bbMin;
}

// destructor
Entity::~Entity()
{

}


// = operator overload
Entity & Entity::operator =(const Entity &source)
{
  if (this != &source)
  {
    m_yaw = source.m_yaw;
    m_pitch = source.m_pitch;
    m_position = source.m_position;
    m_velocity = source.m_velocity;
    //acceleration = source.acceleration;
    m_initialized = source.m_initialized;
    m_bbMax = source.m_bbMax;
    m_bbMin = source.m_bbMin;
  }
  return(*this);
}

int Entity::vcol(const Vec3D &center, float radius)
{
  
  return sphereAABBIntersect(center, radius, m_bbMax + m_position, 
                             m_bbMin + m_position);
}

void Entity::vdestroy(void)
{

  return;
}
int Entity::vinit(void)
{

  return 1;
}
void Entity::vrender(void)
{

  return;
} 

// ***********************************************
Vec3D & Entity::getBBoxMax(void)
{
  return m_bbMax;
}

Vec3D & Entity::getBBoxMin(void)
{
  return m_bbMin;
}

// ************************************************************
// setPosition
int Entity::setPosition(const Vec3D &pos, float yaw, float pitch)
{
  m_position = pos;
  m_yaw = yaw;
  m_pitch = pitch;
  
  return 1;
}
// **************************************************************
// protected functions
// *******************

int Entity::setBBox(Vec3D &max, Vec3D &min)
{
  m_bbMax = max;
  m_bbMin = min;
  return 1;
}

