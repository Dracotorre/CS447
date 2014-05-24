// PillarA.h
/*
    David Shrock
    CS447 - PSU
    2004.07.26
	
    header for PillarA.cpp
    extends Entity

    PillarA is an Entity containing cell decomposition of several solids and
    combined with some other solids of different colors. The cell decomposition
    contains two DSHyperboloids and eight DSCylinders of a stone color. 
    See the sketch in overview.doc.

    Height is 5.6 units.
*/

#ifndef _PILLAR_A_ENTITY_H
#define _PILLAR_A_ENTITY_H

#include <windows.h>                  // required for gl.h
#include <GL/gl.h>                    // standard OpenGL include
//#include <GL/glu.h>                   // OpenGL utilities
#include "Entity.h"
#include "DS/DSCylinder.h"
#include "DS/DSHyperboloid.h"
#include "DS/DSLathe.h"
#include "DS/DSCells.h"               // for cell decomposition
using ds_cells::DSCells;

class PillarA : public Entity
{
  public:
    PillarA();                        // default constructor, length=width = 10
    //PillarA(const Vec3D &);         // constructor to set initial position
    PillarA(const PillarA &);         // copy constructor
    ~PillarA();                       // destructor
    PillarA & operator =(const PillarA &); // assigment

    // not redefining base vcol
    int vinit(void);
    // send ptr to image, image width, height
    int vinit(unsigned char *, GLuint, GLuint);
    void vdestroy(void);                // unload entity data from memory
    void vrender(void);                 // draw the entity


  private:
    unsigned char *m_texImage1;        // image ptr for stone texture
    GLuint         m_stoneID;          // id of our image for glGenTextures
    GLuint         m_dList1;           // Used for the Display List
    DSLathe       *m_torus;
    DSCells       *m_pil;

};
#endif
