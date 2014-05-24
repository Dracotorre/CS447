// DSHelix.h
/*
    David Shrock
    CS447 - PSU
    2004.07.21
	
    implementation of DSHelix.
*/
#include "DSHelix.h"
#include <windows.h>					  // windows constants, required for gl.h
#include <GL/gl.h>						  // standard OpenGL include - for poly drawing
#include <cmath>
using std::cos;
using std::sin;
#include "../Vec3D.h"
#include "../Mat3D.h"
#include "../consts.h"          // for PI

// *************************************
// default constructor
DSHelix::DSHelix()
{
  Vertex bbmin, bbmax;
  m_ht = 2.0f;
  m_coils = 4;
  m_majRadius = 1.0f;
  m_minRadius = 0.1f;

  bbmin.x = bbmin.z = -1.1f;
  bbmax.x = bbmax.z = 1.1f;
  bbmin.y = 0.0f;
  bbmax.y = 2.1f;
  setBBmax(bbmax);
  setBBmin(bbmin);
  m_verts = NULL;
  m_numVerts = 0;
}

// *****************************************
// primary constructor
DSHelix::DSHelix(float radius, float circleRad, int coils, float ht)
{
  Vertex bbmin, bbmax;
  if (radius > 0.00f) m_majRadius = radius;
  else m_majRadius = 1.0f;
  if (circleRad > 0.0f && circleRad < (0.25 * radius)) m_minRadius = circleRad;
  else m_minRadius = 0.10f;
  if (coils > 0) m_coils = coils;
  else m_coils = 4;
  if (ht > 0.0f) m_ht = ht;
  else m_ht = 2.0f;

  // set bounding box
  bbmin.x = bbmin.z = -1 * (m_majRadius + m_minRadius);
  bbmax.x = bbmax.z = m_majRadius + m_minRadius;
  bbmin.y = 0.0f;
  bbmax.y = m_ht;
  setBBmax(bbmax);
  setBBmin(bbmin);
  m_verts = NULL;
  m_numVerts = 0;
}

// ********************************************
// copy constructor
DSHelix::DSHelix(const DSHelix &source) : DSsolid(source)
{
  m_coils = source.m_coils;                  
  m_ht = source.m_ht;                    
  m_majRadius = source.m_majRadius;              
  m_minRadius = source.m_minRadius;
  m_numVerts = source.m_numVerts;
  if (source.m_verts)
  {
    m_verts = new Vec3D[m_numVerts];
    for (int i = 0; i < m_numVerts; ++i)
    {
      m_numVerts[i] = source.m_numVerts[i];
    }
  }
  else m_numVerts = NULL;
}

// ***********************************************
// destructor
DSHelix::~DSHelix()
{
 if (m_verts) delete [] m_verts;
}

// *****************************************************
// assignment op
DSHelix & DSHelix::operator =(const DSHelix &source)
{
  if (this != &source)
  {
    static_cast<DSsolid &>(*this) = source;        // call parent
    m_coils = source.m_coils;                  
    m_ht = source.m_ht;                    
    m_majRadius = source.m_majRadius;              
    m_minRadius = source.m_minRadius;
    m_numVerts = source.m_numVerts;
    if (source.m_verts)
    {
      m_verts = new Vec3D[m_numVerts];
      for (int i = 0; i < m_numVerts; ++i)
      {
        m_numVerts[i] = source.m_numVerts[i];
      }
    }
    else m_numVerts = NULL;
  }
  return(*this);
}

// ************************************************************************
// vdraw
// drawing one step at a time following curve
int DSHelix::vdraw(void)
{
  
  







  return 1;
}

// ************************************************************************
// buildVerts
// vector equation of Helix: r(t) = 2cos(ti) + tk + sin(tj)
// parametric equation:      x = 2 cos t, y = sin t, z = t
void DSHelix::buildVerts(void)
{
  int i = 0;
  int segs = 8;                                 // segments per coil
  int steps = segs * m_coils;           
  double delta_t = m_ht / m_coils;              // as in parametric equation
  double t = 0.0;                               // starting t
  double x = 0.0;                               // as in parametric equation
  double y = 0.0;
  Mat3D rot = rot_z(TWO_PI / HELIX_POLY_VERTS);
  Vec3D pt(m_minRadius, 0.0f, 0.0f);
  Vertex circVerts[HELIX_POLY_VERTS];           // for Poly
  

  m_numVerts = segs * m_coils * HELIX_POLY_VERTS;

  for (i = 0; i < HELIX_POLY_VERTS; ++i)
  {
    circVerts[i].x = pt[0];
    circVerts[i].y = pt[1];
    circVerts[i].z = pt[2];
    m_verts[i] = pt;
    pt = pt * rot;
  }

  if (m_verts) delete [] m_verts;
  m_verts = new Vec3D[m_numVerts];

  Poly circle(circVerts[0], circVerts[1], circVerts[2], HELIX_POLY_VERTS);
  for (i = 3; i < HELIX_POLY_VERTS; ++i)
  {
    circle.addPoint(circVerts[i].x);
  }

  

  // position circle
  rot = rot_x(PI_OVER_4);
  circle.rot(rot);
  circle.translate(Vec3D(m_majRadius, 0.0f, 0.0f));

  for (i = 1; i < steps; ++i)
  {
    t += delta_t / segs;            // parametric equation of helix
    x = 2 * cos(t);
    y = sin(t);
  }








  return;
}