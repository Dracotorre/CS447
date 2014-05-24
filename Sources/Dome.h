// Dome.h
/*
    David Shrock
    CS447 - PSU
    2004.07.29
	
    header for Dome.cpp
    extends Entity

    This is a domed ceiling held up by four PillarA entities and some Arch1.

    Not finished - no dome portion
*/
#ifndef _DOME_ENTITY_H
#define _DOME_ENTITY_H

#include <windows.h>                  // required for gl.h
#include <GL/gl.h>                    // standard OpenGL include
#include <GL/glu.h>                   // OpenGL utilities
#include "Entity.h"
#include "PillarA.h"
#include "Arch1.h"

class Dome : public Entity
{
  public:
    Dome();                           // constructor
    Dome(const Dome &);              // copy constructor
    ~Dome();                          // destructor
    Dome & operator =(const Dome &); // assigment

    // collision testing, send sphere relative to parent coords
    int vcol(const Vec3D &, float);
    //int vinit(void);
    // send image info for: stone and mural
    int vinit(unsigned char *, GLuint, GLuint,
              unsigned char *, GLuint, GLuint);

    void vdestroy(void);                // unload entity data from memory
    void vrender(void);                 // draw the entity


  private:
    unsigned char *m_texImage1;        // image ptr for stone texture
    unsigned char *m_texImage2;        // for mural
    GLuint         m_stoneID;          // for stone id in glGenTextures
    GLuint         m_muralID;          // id of our image for glGenTextures
    GLuint         m_dList1;           // Used for the Display List
    PillarA       *m_pillars;
    Arch1         *m_arches;

};
#endif