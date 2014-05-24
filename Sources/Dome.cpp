// Dome.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.29
	
    implementation of Dome Entity.
*/
#include "Dome.h"
#include "Vec3D.h"
#include "Mat3D.h"
#include "consts.h"            // for PI
#include "materials.h"         // for gold material
#include "intersections.h"
#include <windows.h>            // windows constants, required for gl.h
#include <GL/gl.h>              // standard OpenGL include - for poly drawing

// default constructor
Dome::Dome()
{
  m_texImage1 = NULL;
  m_texImage2 = NULL;
  m_muralID = -1;
  m_stoneID = -1;
  m_dList1= -1;
  m_pillars = NULL;
  m_arches = NULL;
}

// copy constructor
Dome::Dome(const Dome &source)  : Entity(source)
{
  // don't copy, must initialize
  m_texImage1 = NULL;
  m_texImage2 = NULL;
  m_muralID = -1;
  m_stoneID = -1;
  m_dList1 = -1;
  m_pillars = NULL;
  m_arches = NULL;
  m_initialized = -1;
}

// ***************************
// destructor
Dome::~Dome()
{
  if (m_pillars) delete [] m_pillars;
  if (m_arches) delete [] m_arches;
}

// **************************************************
// = operator overload
Dome & Dome::operator =(const Dome &source)
{
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent

    // don't copy, must initialize
    m_texImage1 = NULL;
    m_texImage2 = NULL;
    m_muralID = -1;
    m_stoneID = -1;
    m_dList1 = -1;
    m_pillars = NULL;
    m_arches = NULL;
    m_initialized = -1; 
  }
  return(*this);
}

// *****************************************************************
// col - collision testing
int Dome::vcol(const Vec3D &cent, float radius)
{
  int result = -1;
  // test dome to see if inside area
  if (sphereAABBIntersect(cent, radius, getBBoxMax() + m_position, 
                             getBBoxMin() + m_position) == 1)
  {
    if (m_pillars)
    {
      result = m_pillars[0].vcol(cent, radius);
      if (result == 1) return result;
      result = m_pillars[1].vcol(cent, radius);
      if (result == 1) return result;
      result = m_pillars[2].vcol(cent, radius);
      if (result == 1) return result;
      result = m_pillars[3].vcol(cent, radius);
      if (result == 1) return result;
    }
  }
  return result;
}

// ***************************************************************
// destroy
// unload entity data from memory
void Dome::vdestroy(void)
{
  m_texImage1 = NULL;
  m_texImage2 = NULL;
  m_initialized = 0;

  return;
}

// ****************************************************************
// init
// load data into memory including textures and build the
// display lists for rendering
//
int Dome::vinit(unsigned char *ptr_stone, GLuint stWidth, GLuint stHeight,
                unsigned char *ptr_mural, GLuint muWidth, GLuint muHeight) 
{ 
  Vec3D vecsA[6];            // trailing vertices
  Vec3D vecsB[6];            // leading vertices
  Vec3D vecC;                // the ceiling end point
  Vec3D vec1, vec2, norm;    // for normal calc
  Mat3D rotation;            // for rotating vertices
  int i = 0, j = 0;
  float ceilingAmbient[] = { 0.08f, 0.08f, 0.08f, 1.00f };
  float ceilingDiff[]    = { 0.32f, 0.26f, 0.24f, 1.00f };
  float ceilingColor[]   = { 0.30f, 0.20f, 0.20f, 1.00f };

  m_texImage1 = ptr_stone;

  // build textures if requested
  if (ptr_mural)
  {
    // update the pointer to texture image
  
    m_texImage2 = ptr_mural;

    // setup texture parameters
    glGenTextures(1, &m_muralID);
    glBindTexture(GL_TEXTURE_2D, m_muralID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      //              GL_NEAREST_MIPMAP_LINEAR);
	                 GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, stWidth, stHeight, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, m_texImage2);
  }
  
  // init dome vert locations
  vecsA[0] = Vec3D(-3.0f, 2.0f, 0.0f);
  vecsB[0] = vecsA[0];
  vecC = Vec3D(-16.0f, 2.0f, 0.0f);
  rotation = rot_z(-2.1932454224643);      // rotate dome curve upwards
  for (i = 1; i < 6; ++i)
  {
    vecsA[i] = vecsA[i - 1] * rotation;
    vecsB[i] = vecsA[i];
  }



  /*
  // generate list for dome portion of Dome             not finished
  m_dList1=glGenLists(1);
  glNewList(m_dList1, GL_COMPILE);
    


  glEndList();
  */
  
  // place pillars and arches
  if (!m_pillars)
  {
    m_pillars = new PillarA[4];
    m_pillars[0].vinit(m_texImage1, stWidth, stHeight);
    m_pillars[1].vinit(m_texImage1, stWidth, stHeight);
    m_pillars[2].vinit(m_texImage1, stWidth, stHeight);
    m_pillars[3].vinit(m_texImage1, stWidth, stHeight);

    m_pillars[0].setPosition(Vec3D(-3.5f, 0.0f, -3.5f));
    m_pillars[1].setPosition(Vec3D(-3.5f, 0.0f,  3.5f));
    m_pillars[2].setPosition(Vec3D( 3.5f, 0.0f, -3.5f));
    m_pillars[3].setPosition(Vec3D( 3.5f, 0.0f,  3.5f));
  }
  if (!m_arches)
  {
    m_arches = new Arch1[4];
    
    m_arches[0].vinit(m_texImage1, stWidth, stHeight);
    m_arches[1].vinit(m_texImage1, stWidth, stHeight);
    m_arches[2].vinit(m_texImage1, stWidth, stHeight);
    m_arches[3].vinit(m_texImage1, stWidth, stHeight);

    m_arches[0].setPosition(Vec3D(0.0f, 5.60f, 3.5f));
    m_arches[1].setPosition(Vec3D(0.0f, 5.60f, 3.5f), 180.0f);
    m_arches[2].setPosition(Vec3D(0.0f, 5.60f, 3.5f), -90.0f);
    m_arches[3].setPosition(Vec3D(0.0f, 5.60f, 3.5f), 90.0f);
  }



  // update bounding box for this Entity
  // making a little larger since can walk inside
  setBBox(Vec3D(4.1f, 7.8f, 4.1f), Vec3D(-4.1f, 0.0f, -4.1f));

  m_initialized = 1;

  return m_initialized;
}


// ***********************************************************
// render
// *********************************************************
void Dome::vrender(void)
{
  if (m_initialized == 1)
  {
    glPushMatrix();
      if (m_position != Vec3D(0.0f, 0.0f, 0.0f))
      {
        glTranslatef(m_position[0], m_position[1], m_position[2]);
      }
      if (m_yaw != 0.0f) 
      {
        glRotatef(m_yaw, 0.0f, 1.0f, 0.0f);
      }
      // skipping pitch

      //glCallList(m_dList1);

      m_pillars[0].vrender();
      m_pillars[1].vrender();
      m_pillars[2].vrender();
      m_pillars[3].vrender();
      m_arches[0].vrender();
      m_arches[1].vrender();
      m_arches[2].vrender();
      m_arches[3].vrender();
    glPopMatrix();
  }
  return;
}