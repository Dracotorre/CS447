//TorStand.h
/*
   David Shrock
   CS447 - PSU
   2004.08.01

   header for TorStand.cpp
   extends Entity

   Stand for the Fun Torus to sit on. Made of stacked DSHyperboloid
   using cell decomposition.
*/
#ifndef _TORSTAND_ENTITY_H
#define _TORSTAND_ENTITY_H

#include <windows.h>                  // required for gl.h
#include <GL/gl.h>                    // standard OpenGL include
#include "Entity.h"
#include "DS/DSHyperboloid.h"
#include "FunTorus.h"

class TorStand : public Entity
{
  public:
    TorStand();                              // constructor
    TorStand(const TorStand &);              // copy constructor
    ~TorStand();                             // destructor
    TorStand & operator =(const TorStand &); // assigment

    // not redifining vcol, letting base do collision testing

    int vinit(void);
    void vdestroy(void);                // unload entity data from memory
    void vrender(void);                 // draw the entity

  private:
    GLuint         m_dList1;           // Used for the Display List
    DSHyperboloid *m_bot;
    DSHyperboloid *m_top;
    FunTorus      *m_torus;


};
#endif
