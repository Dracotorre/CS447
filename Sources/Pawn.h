// Pawn.h
/*
    David Shrock
    CS447 - PSU
    2004.08.05

    header for Pawn.cpp
    extends Entity
*/
#ifndef _PAWN_ENTITY_H
#define _PAWN_ENTITY_H

#include <windows.h>                  // required for gl.h
#include <GL/gl.h>                    // standard OpenGL include
#include <GL/glu.h>                   // OpenGL utilities
#include "Entity.h"
#include "DS/DSLathe.h"

class Pawn : public Entity
{
  public:
    Pawn();                           // constructor
    Pawn(const Pawn &);              // copy constructor
    ~Pawn();                          // destructor
    Pawn & operator =(const Pawn &); // assigment

    // not redefining base vcol
    int vinit(void);
    // send ptr to image for stand portion, image width, height
    int vinit(unsigned char *, GLuint, GLuint);
    void vdestroy(void);                // unload entity data from memory
    void vrender(void);                 // draw the entity


  private:
    unsigned char *m_texImage1;        // image ptr for stone texture
    GLuint         m_texID;            // id of our image for glGenTextures
    GLuint         m_dList1;           // Used for the Display List
    DSLathe       *m_pawn;           
};
#endif
