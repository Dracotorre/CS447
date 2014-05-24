// DSsolid.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.20
	
    implementation of DSsolid
*/
#include "DSsolid.h"              // also includes Vec3D, Poly, Vertex
#include <windows.h>					  // windows constants, required for gl.h
#include <GL/gl.h>						  // standard OpenGL include - for poly drawing

// constructor
DSsolid::DSsolid()
{
  m_bbmin = NULL;
  m_bbmax = NULL;
  m_rotation = NULL;
  m_translation = NULL;
  m_orig = NULL;
  m_hideFace = new short[6];
  for (int i = 0; i < 6; ++i)
  {
    m_hideFace[i] = 0;
  }
}

// **************************
// copy constructor
DSsolid::DSsolid(const DSsolid &source)
{
  int i = 0;
  if (source.m_bbmax) 
  {
    m_bbmax = new Vertex;
    m_bbmax->x = source.m_bbmax->x;
    m_bbmax->y = source.m_bbmax->y;
    m_bbmax->z = source.m_bbmax->z;
  }
  else m_bbmax = NULL;

  if (source.m_bbmin)
  {
    m_bbmin = new Vertex;
    m_bbmin->x = source.m_bbmin->x;
    m_bbmin->y = source.m_bbmin->y;
    m_bbmin->z = source.m_bbmin->z;
  }
  else m_bbmin = NULL;

  if (source.m_rotation) 
  {
    m_rotation = new Mat3D(*source.m_rotation);
  }
  else m_rotation = NULL;
  if (source.m_translation)
  {
    m_translation = new Vec3D(*source.m_translation);
  }
  else m_translation = NULL;
  if (source.m_orig)
  {
    m_orig = new Vertex[2];
    m_orig[0].x = source.m_orig[0].x;
    m_orig[0].y = source.m_orig[0].y;
    m_orig[0].z = source.m_orig[0].z;
    m_orig[1].x = source.m_orig[1].x;
    m_orig[1].y = source.m_orig[1].y;
    m_orig[1].z = source.m_orig[1].z;
  }
  else m_orig = NULL;

  m_hideFace = new short[6];
  if (source.m_hideFace)
  {
    for (i = 0; i < 6; ++i)
    {
      m_hideFace[i] = source.m_hideFace[i];
    }
  }
  else
  {
    for (i = 0; i < 6; ++i)
    {
      m_hideFace[i] = 0;
    }
  }
}

// **************************
// destructor
DSsolid::~DSsolid() 
{
  if (m_bbmax) delete m_bbmax;
  if (m_bbmin) delete m_bbmin;
  if (m_rotation) delete m_rotation;
  if (m_translation) delete m_translation;
  if (m_orig) delete [] m_orig;
  if (m_hideFace) delete [] m_hideFace;
}

// *******************************
// assignment op
DSsolid & DSsolid::operator =(const DSsolid &source)
{
  int i = 0;

  if (this != &source)
  {
    if (source.m_bbmax) 
    {
      m_bbmax = new Vertex;
      m_bbmax->x = source.m_bbmax->x;
      m_bbmax->y = source.m_bbmax->y;
      m_bbmax->z = source.m_bbmax->z;
    }
    else m_bbmax = NULL;

    if (source.m_bbmin)
    {
      m_bbmin = new Vertex;
      m_bbmin->x = source.m_bbmin->x;
      m_bbmin->y = source.m_bbmin->y;
      m_bbmin->z = source.m_bbmin->z;
    }
    else m_bbmin = NULL;

    if (source.m_rotation) 
    {
      m_rotation = new Mat3D(*source.m_rotation);
    }
    else m_rotation = NULL;
    if (source.m_translation)
    {
      m_translation = new Vec3D(*source.m_translation);
    }
    else m_translation = NULL;
    if (source.m_orig)
    {
      m_orig = new Vertex[2];
      m_orig[0].x = source.m_orig[0].x;
      m_orig[0].x = source.m_orig[0].y;
      m_orig[0].x = source.m_orig[0].z;
      m_orig[1].x = source.m_orig[1].x;
      m_orig[1].y = source.m_orig[1].y;
      m_orig[1].z = source.m_orig[1].z;
    }
    else m_orig = NULL;
    m_hideFace = new short[6];
    if (source.m_hideFace)
    {
      for (i = 0; i < 6; ++i)
      {
        m_hideFace[i] = source.m_hideFace[i];
      }
    }
    else
    {
      for (i = 0; i < 6; ++i)
      {
        m_hideFace[i] = 0;
      }
    }
  }
  return(*this);
}

// **********************************************************************
int DSsolid::getBBmax(Vertex &vert) 
{
  if (m_bbmax) 
  {
    vert.x = m_bbmax->x;
    vert.y = m_bbmax->y;
    vert.z = m_bbmax->z;
  }
  else 
  {
    vert.x = vert.y = vert.z = 0.0f;
    return -1;
  }
  return 1;
}

// ************************************************************************    
int DSsolid::getBBmin(Vertex &vert) 
{
  if (m_bbmin) 
  {
    vert.x = m_bbmin->x;
    vert.y = m_bbmin->y;
    vert.z = m_bbmin->z;
  }
  else 
  {
    vert.x = vert.y = vert.z = 0.0f;
    return -1;
  }
  return 1; 
}

// ************************************************************************
int DSsolid::setHideFace(int face)
{
  if (face < 0 || face > 5) return -1;
  m_hideFace[face] = 1;
  return 1;
}

// ************************************************************************
// setRotation
// rotations may end up swapping maxs and mins, so may need to flip
int DSsolid::setRotation(const Mat3D &rot)
{
  Vec3D vec1;

  if (!m_rotation) 
  {
    m_rotation = new Mat3D(rot);
  } 
  else 
  {
    *m_rotation = rot;
  }

  // reset bounding box to origin
  if (m_bbmax && m_orig) 
  {
    m_bbmax->x = m_orig[0].x;
    m_bbmax->y = m_orig[0].y;
    m_bbmax->z = m_orig[0].z;
  }
  if (m_bbmin && m_orig)
  {
    m_bbmin->x = m_orig[1].x;
    m_bbmin->y = m_orig[1].y;
    m_bbmin->z = m_orig[1].z;
  }
  // else doesn't matter - children set on construction
    
  // update the bounding box
  if (m_bbmax)
  {
    vec1 = Vec3D(m_bbmax->x, m_bbmax->y, m_bbmax->z);
    vec1 = vec1 * *m_rotation;
    m_bbmax->x = vec1[0];
    m_bbmax->y = vec1[1];
    m_bbmax->z = vec1[2];
  }
  if (m_bbmin)
  {
    vec1 = Vec3D(m_bbmin->x, m_bbmin->y, m_bbmin->z);
    vec1 = vec1 * *m_rotation;
    m_bbmin->x = vec1[0];
    m_bbmin->y = vec1[1];
    m_bbmin->z = vec1[2];
  }
  checkOrientation();           // check correctness

  return 1;
}
    
// ***********************************************************************
int DSsolid::setTranslation(const Vec3D &trans)
{
  Vec3D vec1;

  if (!m_translation) 
  {
    m_translation = new Vec3D(trans);
  }   
  else 
  {
    *m_translation = trans;
  }

  // reset bounding box to origin
  if (m_bbmax && m_orig) 
  {
    m_bbmax->x = m_orig[0].x;
    m_bbmax->y = m_orig[0].y;
    m_bbmax->z = m_orig[0].z;
  }
  if (m_bbmin && m_orig)
  {
    m_bbmin->x = m_orig[1].x;
    m_bbmin->y = m_orig[1].y;
    m_bbmin->z = m_orig[1].z;
  }
    // else doesn't matter - children set on construction

  // update the bounding box

  if (m_bbmax)
  {
    vec1 = Vec3D(m_bbmax->x, m_bbmax->y, m_bbmax->z);
    if (m_rotation) vec1 = vec1 * *m_rotation;
    vec1 += *m_translation;
    m_bbmax->x = vec1[0];
    m_bbmax->y = vec1[1];
    m_bbmax->z = vec1[2];
  }
  if (m_bbmin)
  {
    vec1 = Vec3D(m_bbmin->x, m_bbmin->y, m_bbmin->z);
    if (m_rotation) vec1 = vec1 * *m_rotation;
    vec1 += *m_translation;
    m_bbmin->x = vec1[0];
    m_bbmin->y = vec1[1];
    m_bbmin->z = vec1[2];
  }
  checkOrientation();             // make sure max is max, min is min
  return 1;
}

// **************************************************************************
// draw
int DSsolid::vdraw(void)
{

  return 1;
}

// ************************************************************************
// protected functions
// *****************************

// **************************************************************************
// drawPoly
// draws the polygon - best for non-quads non-triangles
int DSsolid::drawPoly(Poly &polygon, int texType)
{
  int i = 0;
  int polySize = polygon.size();
  Vertex tPoint;                          // temporary vertex holder
  Vec3D normal = polygon.normal();        // for surface normal calc
  GLfloat texX = 0.0f, texY = 0.0f;       // texture coords
  float halfway = 0.0, quarterway = 0.0;  // for calc poly textures
  float index = 0.0;                      // copy of i

  // draw polygon
  glBegin(GL_POLYGON);
  glNormal3f(normal[0], normal[1], normal[2]);
  halfway = polySize * 0.500f;
  quarterway = polySize * 0.250f;
  for (i = 0; i < polySize; ++i)
  {
    if (texType == 1) 
    {
      index = (float)i;                         // conversion slow, do once

      // texture coords start at (0,0) to (1,0) at 1/4way, (1,1) at halfway
      if (i <= halfway && i >= quarterway) 
      {
        texX = 1.0f;
        texY = (GLfloat)((quarterway - (halfway - index)) / quarterway);
      }
      else if (i < quarterway) 
      {
        texX = (GLfloat)(index / quarterway);
        texY = 0.0f;
      }
      else if (i > halfway && i <= halfway + quarterway)
      {
        texX = (GLfloat)((halfway + quarterway - index) / quarterway);
        texY = 1.0f;
      }
      else 
      {
        texX = 0.0f;
        texY = (GLfloat)((polySize - (index) / quarterway));
      }
      glTexCoord2f(texX, texY);
    }
    tPoint = polygon[i];
    glVertex3f(tPoint.x, tPoint.y, tPoint.z);
  }
  glEnd();

  return 1;
}

// ************************************************************************
// setBBMax 
int DSsolid::setBBmax(Vertex &vert)
{
  if (!m_bbmax) m_bbmax = new Vertex;
  m_bbmax->x = vert.x;
  m_bbmax->y = vert.y;
  m_bbmax->z = vert.z;
  if (!m_orig) m_orig = new Vertex[2];
  m_orig[0].x = m_bbmax->x;
  m_orig[0].y = m_bbmax->y;
  m_orig[0].z = m_bbmax->z;

  return 1;
}

// ***********************************************************************
// setBBmin
int DSsolid::setBBmin(Vertex &vert)
{
  if (!m_bbmin) m_bbmin = new Vertex;
  m_bbmin->x = vert.x;
  m_bbmin->y = vert.y;
  m_bbmin->z = vert.z;
  if (!m_orig) m_orig = new Vertex[2];
  m_orig[1].x = m_bbmin->x;
  m_orig[1].y = m_bbmin->y;
  m_orig[1].z = m_bbmin->z;
  
 
  return 1;
}

// *******************************************************************
// private functions
// ********************

// **********************************************************************
// orientations of max and min may get reversed on rotation, so swap if
void DSsolid::checkOrientation(void)
{
  float temp = 0.0f;

  if (m_bbmax && m_bbmin)
  {
    if (m_bbmax->x < m_bbmin->x)
    {
      temp = m_bbmax->x;
      m_bbmax->x = m_bbmin->x;
      m_bbmin->x = temp;
    }
    if (m_bbmax->y < m_bbmin->y)
    {
      temp = m_bbmax->y;
      m_bbmax->y = m_bbmin->y;
      m_bbmin->y = temp;
    }
    if (m_bbmax->z < m_bbmin->z)
    {
      temp = m_bbmax->z;
      m_bbmax->z = m_bbmin->z;
      m_bbmin->z = temp;
    }
  }
  return;
}

