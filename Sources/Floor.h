// Floor.h
/*
    David Shrock
    CS447 - PSU
    2004.07.10

    header for Floor.cpp

    Extends Entity to be any floor in the world defined as a flat rectangular
    surface with orientation always facing up. Floor texture is selectable
    from a built in list of floor textures that are available for this project.
*/

#ifndef _FLOOR_ENTITY_H
#define _FLOOR_ENTITY_H

#include <windows.h>            // required for gl.h
#include <GL/gl.h>	        // standard OpenGL include
#include <GL/glu.h>	        // OpenGL utilities, for mipmaps
#include "Entity.h"
#include "imageFile.h"

class Floor : public Entity
{
  public:
    Floor();                        // default constructor, length=width = 10
    Floor(int, int);                // constructor to set initial length, width
    //Floor(const Vec3D &);         // constructor to set initial position
    Floor(const Floor &);              // copy constructor
    ~Floor();                          // destructor
    Floor & operator =(const Floor &); // = operator overload

    // initialize, send ptr to texture image data, returns 1 for ok
    int vinit(unsigned char *, GLuint width, GLuint height);
    void vdestroy(void);                // unload entity data from memory
    void vrender(void);                 // draw the entity


  private:
    unsigned char *m_texImage;
    GLuint    m_texID;                  // id of our image for glGenTextures
    GLuint    m_rect1;                  // Used for the Cube Display List
    int       m_length;                 // length of floor (x-direction)
    int       m_width;                  // width of floor (z-direction)
};

#endif
