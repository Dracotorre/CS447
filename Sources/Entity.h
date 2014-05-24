// Entity.h
/*
   David Shrock
   CS447 - PSU
   2004.07.05

   header for Entity.cpp

   Base class definition for an entity (object) in the world that can be
   interacted with, such as a wall, door, chair, vase, person, etc. All
   entities in the world should be derived from this class.
*/

#ifndef _BASE_ENTITY_H
#define _BASE_ENTITY_H
#include "Vec3D.h"

class Entity
{
  public:
    Entity();                            // default constructor
    Entity(const Entity &);              // copy constructor
    virtual ~Entity();                   // destructor
    Entity & operator =(const Entity &); // = operator overload

    // collision testing, send sphere center and radius. Some objects
    //   may also do internal collision testing.
    virtual int vcol(const Vec3D &, float);      
    virtual int vinit(void);              // load entity data into memory
    virtual void vdestroy(void);          // unload entity data from memory
    virtual void vrender(void);           // draw the object
    

    Vec3D & getBBoxMax(void);             // get bounding box
    Vec3D & getBBoxMin(void);

    // yaw and pitch are in degrees
    int setPosition(const Vec3D &, float yaw=0.0, float pitch=0.0);

    // animate the object, includes physics calculations
    // virtual void animate(scalarT);

  protected:
    int    m_initialized;                  // if entity is ready to go
    float  m_yaw;                          // angle of orientation
    float  m_pitch;                        // angle
    Vec3D  m_position;                     // object position
    Vec3D  m_velocity;                     // velocity of object
    //Vec3D acceleration;                  // acceleration

    int setBBox(Vec3D &max, Vec3D &min);
     
  private:
    Vec3D m_bbMax;                         // bounding box corners
    Vec3D m_bbMin;  
};

#endif
