// LampA.h
/*
    David Shrock
    CS447 - PSU
    2004.07.27

    header for LampA.cpp
    extends Entity

    LampA: tall floor standing lamp. If using a light source with this, the lamp
    shade stands at 1.6 units above origin in +y.
*/
#ifndef _LAMP_A_ENTITY_H
#define _LAMP_A_ENTITY_H

#include <windows.h>                  // required for gl.h
#include <GL/gl.h>                    // standard OpenGL include
#include <GL/glu.h>                   // OpenGL utilities
#include "Entity.h"
#include "DS/DSCylinder.h"
#include "DS/DSHyperboloid.h"
#include "DS/DSCells.h"               // for cell decomposition
using ds_cells::DSCells;

class LampA : public Entity
{
  public:
    LampA();                           // constructor
    LampA(const LampA &);              // copy constructor
    ~LampA();                          // destructor
    LampA & operator =(const LampA &); // assigment

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
    DSCells       *m_stand;            // stand portion uses Cell Decomp

};
#endif
