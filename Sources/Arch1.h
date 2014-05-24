// Arch1.h
/*
    David Shrock
    CS447 - PSU
    2004.07.28
	
    header for Arch1.cpp
    extends Entity

    An arch that may hold up a ceiling while standing on pillars. Arch1 is 
    designed to connect to others at 90 degree corners, so the backside
    is mitered at each end. 

    Line up pillars 3.5 distance from origin in x axis.
*/
#ifndef _ARCH1_ENTITY_H
#define _ARCH1_ENTITY_H

#include <windows.h>                  // required for gl.h
#include <GL/gl.h>                    // standard OpenGL include
#include <GL/glu.h>                   // OpenGL utilities
#include "Entity.h"

class Arch1 : public Entity
{
  public:
    Arch1();                           // constructor
    Arch1(const Arch1 &);              // copy constructor
    ~Arch1();                          // destructor
    Arch1 & operator =(const Arch1 &); // assigment

    // not redifining vcol, letting base do collision testing

    //int vinit(void);
    // send image info
    int vinit(unsigned char *, GLuint, GLuint);
    void vdestroy(void);                // unload entity data from memory
    void vrender(void);                 // draw the entity


  private:
    unsigned char *m_texImage1;        // image ptr for stone texture
    GLuint         m_stoneID;          // id of our image for glGenTextures
    GLuint         m_dList1;           // Used for the Display List

};
#endif