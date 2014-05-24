// FunTorus.h
/*
   David Shrock
   CS447 - PSU
   2004.07.29

   header for FunTorus.cpp
   extends Entity

   Spinning diamond shaped torus.
*/
#ifndef _FUN_TORUS_ENTITY_H
#define _FUN_TORUS_ENTITY_H

#include <windows.h>                  // required for gl.h
#include <GL/gl.h>                    // standard OpenGL include
#include "Entity.h"
#include "DS/DSLathe.h"

class FunTorus : public Entity
{
  public:
    FunTorus();                           // constructor
    FunTorus(const FunTorus &);              // copy constructor
    ~FunTorus();                          // destructor
    FunTorus & operator =(const FunTorus &); // assigment

    // not redifining vcol, letting base do collision testing

    int vinit(void);
    void vdestroy(void);                // unload entity data from memory
    void vrender(void);                 // draw the entity

  private:
    GLuint         m_dList1;           // Used for the Display List

};
#endif
