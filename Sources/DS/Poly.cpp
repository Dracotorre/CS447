// Poly.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.18
	
    implementation of Poly. (2 pt lines allowed)

    Default polygon is a circle.
*/
#include "Poly.h"
#include "../consts.h"                  // for PI
#include <cmath>
using std::cos;
using std::sin;

// default constructor produces a circle
Poly::Poly()
{
  circle();
  m_onZ = 1;
}

// ************************************************
// primary constructor with 3 points to start off. 
// total is the number of points caller plans on putting in, but doesn't
// have to since m_size will keep track of it.
//
Poly::Poly(const Vertex &pt1, const Vertex &pt2, const Vertex &pt3, int total)
{
  int check = 0;                          // for duplicate checking

  // make sure no duplicates
  if (pt2.x == pt1.x && pt2.y == pt1.y && pt2.z == pt1.z)
  {
    check = -1;
  }
  else if (pt3.x == pt1.x && pt3.y == pt1.y && pt3.z == pt1.z)
  {
    check = -1;
  }
  else if (pt3.x == pt2.x && pt3.y == pt2.y && pt3.z == pt2.z)
  {
    check = -1;
  }

  if (check == -1) circle();              // if duplices then do circle
  else
  {
    // no dupes so update
    if (total < 3) m_maxSize = 3;           // force boundries
    else if (total > 112) m_maxSize = 112;
    else m_maxSize = total;
    m_size = 3;                             // set to current size
    m_verts = new Vertex[m_maxSize];

    m_verts[0].x = pt1.x;
    m_verts[0].y = pt1.y;
    m_verts[0].z = pt1.z;
    m_verts[1].x = pt2.x;
    m_verts[1].y = pt2.y;
    m_verts[1].z = pt2.z;
    m_verts[2].x = pt3.x;
    m_verts[2].y = pt3.y;
    m_verts[2].z = pt3.z;

    // init any remaining verts to 0
    //  These aren't part of the polygon yet, only up to m_size is.
    for (int i = 3; i < m_maxSize; ++i)
    {
      m_verts[i].x = 0.0f;
      m_verts[i].y = 0.0f;
      m_verts[i].z = 0.0f;
    }

    // find normal of this polygon
    Vec3D vec1(m_verts[1].x - m_verts[0].x, 
               m_verts[1].y - m_verts[0].y,
               m_verts[1].z - m_verts[0].z);
    Vec3D vec2(m_verts[2].x - m_verts[0].x,
               m_verts[2].y - m_verts[0].y,
               m_verts[2].z - m_verts[0].z);
    m_normal = Vec3D(vec1 * vec2);
    m_normal.unitNorm();

    if (m_verts[0].z == m_verts[1].z) m_onZ = 1;
    else m_onZ = 0;
  }
}

// ***********************************************************************
// copy constructor
Poly::Poly(const Poly &source)
{
  m_size = source.m_size;
  m_maxSize = source.m_maxSize;

  if (source.m_verts)
  {
    m_verts = new Vertex[m_maxSize];
    for (int i = 0; i < m_maxSize; ++i)
    {
      m_verts[i].x = source.m_verts[i].x;
      m_verts[i].y = source.m_verts[i].y;
      m_verts[i].z = source.m_verts[i].z;
    }
  }
  else m_verts = NULL;

  m_normal = source.m_normal;
  m_onZ = source.m_onZ;
}

// *****************************************
// destructor
Poly::~Poly()
{
  if (m_verts) delete [] m_verts;
}

// ***********************************************************************
// assignment op
Poly & Poly::operator =(const Poly &source)
{
  if (this != &source)
  {
    m_size = source.m_size;
    m_maxSize = source.m_maxSize;

    if (source.m_verts)
    {
      m_verts = new Vertex[m_maxSize];
      for (int i = 0; i < m_maxSize; ++i)
      {
        m_verts[i].x = source.m_verts[i].x;
        m_verts[i].y = source.m_verts[i].y;
        m_verts[i].z = source.m_verts[i].z;
      }
    }
    else m_verts = NULL;

    m_normal = source.m_normal;
    m_onZ = source.m_onZ;
  }
  return *this;
}

// ***********************************************************************
// [] operator, returns the vertex for requested index
Vertex & Poly::operator [](int index) const
{
  // force boundry
  if (index < 0) index = 0;
  else if (index > m_size) index = m_size;
  return m_verts[index];
}

// ***********************************************************************
// add another point
// non-planar and duplicates are rejected with return -1
int Poly::addPoint(const Vertex &pt)
{
  int check = 1;
  // check for common z first
  if (m_onZ != 1) 
  {
    check = checkPlanar(pt.x, pt.y, pt.z);   
  }
  else if (pt.z == 0.0f) check = 1;

  if (check == 1) check = checkDupes(pt.x, pt.y, pt.z);    
  if (check == 1) addToPoly(pt.x, pt.y, pt.z);
  return check;
}

int Poly::addPoint(const float x, const float y, const float z)
{
  float x2 = x;
  float y2 = y;
  float z2 = z;

  int check = checkPlanar(x2, y2, z2);    
  if (check == 1) check = checkDupes(x2, y2, z2);    // checks for dupes
  if (check == 1) addToPoly(x2, y2, z2);
  return check;
}

// **********************************************************************
// rotate entire Poly
int Poly::rot(const Mat3D &dir)
{
  Vec3D vec;

  for (int i = 0; i < m_size; ++i)
  {
    vec = Vec3D(m_verts[i].x, m_verts[i].y, m_verts[i].z);
    vec = vec * dir;
    m_verts[i].x = vec[0];
    m_verts[i].y = vec[1];
    m_verts[i].z = vec[2];
  }
  return 1;
}

// **********************************************************************
// translate   -entire Poly in direction of vector
int Poly::translate(const Vec3D &dir)
{
  for (int i = 0; i < m_size; ++i)
  {
    m_verts[i].x += dir[0];
    m_verts[i].y += dir[1];
    m_verts[i].z += dir[2];
  }
  return 1;
}

// ***********************************************************************
// private functions
// *************************************

// *********************************
// add to poly
int Poly::addToPoly(float x, float y, float z)
{
  Vertex *temp;                          // copy holder if increase array
  int i = 0;

  // add to end
    if (m_size >= m_maxSize)
    {
      // need to redo dynamic array
      temp = new Vertex[m_maxSize];
      for (i = 0; i < m_maxSize; ++i)
      {
        temp[i].x = m_verts[i].x;
        temp[i].y = m_verts[i].y;
        temp[i].z = m_verts[i].z;
      }
      m_maxSize = m_size + 1;                 // increase max for new data
      delete [] m_verts;                      // delete old array
      m_verts = new Vertex[m_maxSize];        // create new array with new size
      for (i = 0; i < m_size; ++i)            // copy back the temps
      {
        m_verts[i].x = temp[i].x;
        m_verts[i].y = temp[i].y;
        m_verts[i].z = temp[i].z;
      }
    }
    // add to end of list and increase size counter
    m_verts[m_size].x = x;
    m_verts[m_size].y = y;
    m_verts[m_size].z = z;

    ++m_size;

  return 1;

}
// ***********************************************************************
// checks for duplicates 
int Poly::checkDupes(float x, float y, float z)
{
  int check = 1;
  int i = 0;
  
  for (i = 0; i < m_size; ++i)
  {
    if (x == m_verts[i].x && 
        y == m_verts[i].y &&
        z == m_verts[i].z )
    {
      check = -1;
      break;
    }
  } 
  return check;
}

// ***********************************************************************
// checkPlanar
// test to see if pt is planar with Poly
int Poly::checkPlanar(float px, float py, float pz)
{
  int check = 1;
  
  // get normal of new point with last points
  Vec3D vec1 = Vec3D(m_verts[m_size - 1].x - m_verts[m_size - 2].x, 
                     m_verts[m_size - 1].y - m_verts[m_size - 2].y,
                     m_verts[m_size - 1].z - m_verts[m_size - 2].z);
  Vec3D vec2 = Vec3D(px - m_verts[m_size - 2].x, 
                     py - m_verts[m_size - 2].y,
                     pz - m_verts[m_size - 2].z);

  Vec3D testNorm = vec1 * vec2;
  testNorm.unitNorm();
  if (m_normal == testNorm) check = 1;
  else check = -2;
  return check;
}

// ***********************************************************************
// circle
// produces default circle facing +z with radius 1
void Poly::circle(void)
{
  double angle = 0.0;
  m_size = 16;
  m_maxSize = m_size;
  m_verts = new Vertex[m_size];

  for (int i = 0; i < m_size; ++i)
  {
    angle = -2 * PI * i / m_size;
    m_verts[i].x = (scalarT)sin(angle);
    m_verts[i].y = (scalarT)cos(angle);
    m_verts[i].z = 0.0f;
  }
  m_normal = Vec3D(0.0, 0.0, 1.0);

  return;
}
