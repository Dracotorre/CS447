// EndTable1.h
/*
   David Shrock
   CS447 - PSU
   2004.07.29

   header for EndTable1.cpp
   extends Entity

   End table created by lathing. A little 'toy' sits on table to show off
   extrusion. This means, can't be picked up, but it's just for show.

   top height is 1.175
*/
#ifndef _END_TABLE1_ENTITY_H
#define _END_TABLE1_ENTITY_H

#include <windows.h>                  // required for gl.h
#include <GL/gl.h>                    // standard OpenGL include
#include <GL/glu.h>                   // OpenGL utilities
#include "Entity.h"
#include "DS/DSLathe.h"     
#include "DS/DSExtrude.h"

class EndTable1 : public Entity
{
  public:
    EndTable1();                           // constructor
    EndTable1(const EndTable1 &);              // copy constructor
    ~EndTable1();                          // destructor
    EndTable1 & operator =(const EndTable1 &); // assigment

    // not redifining vcol, letting base do collision testing

    int vinit(void);
    void vdestroy(void);                // unload entity data from memory
    void vrender(void);                 // draw the entity

  private:
    unsigned char *m_texImage1;        // image ptr for stone texture
    GLuint         m_texID;            // id of our image for glGenTextures
    GLuint         m_dList1;           // Used for the Display List
    DSLathe       *m_top;
    DSLathe       *m_leg;
    DSExtrude     *m_toy;              // simple extrusion example

    void buildTop(void);               // set points and create
    void buildLeg(void);
    void buildToy(void);

};
#endif