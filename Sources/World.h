// World.h
/*
  David Shrock
  CS447 - PSU
  2004.07.29

  header for World.cpp
  extends entity

  World is an Entity that holds everything including the camera. Since Entity
  contains position, velocity, orientation then it is possible that the world
  could transform relative to some other Entity such as a space ship could be
  seen as its own little world (once someone leaves the ship, then they enter
  a different world). But for the CS447 project, this is everything.

  The EntityList is used as an viewlist to keep track of what needs to be
  rendered depending on camera position and orientation. This would be very
  handy for rooms with many objects to keep track of. Makes it easier to render
  by simply calling the viewlist to render. The EntityList is not finished, so
  is not yet implemented.
*/

#ifndef _WORLD_H
#define _WORLD_H

#include "TgaList.h"               // for Targa image list
#include "Entity.h"
//#include "EntityList.h"
#include "Camera.h"
#include "Floor.h"
#include "Dome.h"
#include "LampA.h"
#include "EndTable1.h"
#include "TorStand.h"
#include "Pawn.h"

class World : public Entity
{
  public:
    World();
    World(const World &);
    ~World();
    // do we really want to copy the World? The universe would get boring.
    World & operator =(const World &);

    // set camera deltas
    // Does collision testing.
    // returns -1 for not set: collision or 1 for set ok
    int moveCam(const Vec3D &deltaPos, float deltaPitch, float deltaYaw);   

    int   vinit(void);             // load entity data into memory, entire world
    void  vdestroy(void);          // unload entity data from memory
    void  vrender(void);           // draw the world

  private:
    int         m_idList[5];        // list of id's in viewList
    GLfloat     m_shadowMatrix[16]; // for shadows
    GLfloat     m_lightPosition[4];
    TgaList    *m_targas;           // list of Targa images for textures
    Floor      *m_floor;
    Camera     *m_cam;  
    Dome       *m_dome;            // dome includes pillars and arches
    LampA      *m_lamp;
    EndTable1  *m_endTable;
    TorStand   *m_torus;           // spinning diamond shaped torus
    Pawn       *m_pawn;
    GLuint      m_dList;           // for some boring walls
    GLuint      m_texID;           // for the boring walls
    unsigned char *m_texImage1;    // for the boring walls 
    //EntityList *m_viewList;       // what's in view that needs to be drawn

    int camCollisionTest(const Vec3D &, const scalarT &);
    void createWalls(unsigned char *, GLuint, GLuint);
    int setShadowMatrix(void);

};
#endif
	
