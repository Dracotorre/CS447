// DSLathe.h
/*
    David Shrock
    CS447 - PSU
    2004.07.19
	
    implementation of DSLathe, lathing an arbitrary polygon.
*/
#include "DSLathe.h"
#include <windows.h>					  // windows constants, required for gl.h
#include <GL/gl.h>						  // standard OpenGL include - for poly drawing
#include "../Vec3D.h"
#include "../Mat3D.h"
#include "../consts.h"          // for PI

// *************************************
// default constructor
DSLathe::DSLathe()
{
  int size = 0;                     // of polygon
  Vertex vert, bbmin, bbmax;

  // translate default Poly into correct spot for default torus
  Vec3D trans(3.0f, 0.0f, 0.0f);
  m_poly.translate(trans);
  m_rot = 1.0f;
  m_slices = 16;

  // find bounding box
  size = m_poly.size();
  vert = m_poly[0];
  bbmax.x = vert.x;
  bbmax.y = bbmin.y = vert.y;
  for (int i = 1; i < size; ++i)
  {
    vert = m_poly[i];
    if (vert.x > bbmax.x) bbmax.x = vert.x;
    if (vert.y > bbmax.y) bbmax.y = vert.y;
    else if (vert.y < bbmin.y) bbmin.y = vert.y;
  }
  bbmin.x = bbmax.x * -1;
  bbmax.z = bbmax.x;
  bbmin.z = bbmax.z * -1;

  setBBmax(bbmax);
  setBBmin(bbmin);
}

// *****************************************
// primary constructor
// good polygon must be on z=0 plane and +x side
DSLathe::DSLathe(const Poly &polygon, int slices, float percent)
{
  Poly temp = polygon;
  int size = temp.size();
  int check = 1;
  Vertex vert, bbmin, bbmax;

  // check to be sure polygon is only on +x and z=0
  for (int i = 0; i < size; ++i)
  {
    vert = temp[i];
    if (vert.x < 0.0f) 
    {
      check = -1;
      break;
    }
    else if (vert.z != 0.0f) 
    {
      check = -1;
      break;
    } 
  }
  if (check == 1)
  {
    // good polygon, so update as requested
    m_poly = polygon;
    if (slices > 2) m_slices = slices;
    else slices = 8;
    if (percent > 0.03f && percent <= 1.00f) m_rot = percent;
    else m_rot = 1.00f;
  }
  else
  {
    // translate default Poly into correct spot for default torus
    Vec3D trans(3.0f, 0.0f, 0.0f);
    m_poly.translate(trans);
    m_rot = 1.0f;
    m_slices = 16;
  }
  // find bounding box
  size = m_poly.size();
  vert = m_poly[0];
  bbmax.x = vert.x;
  bbmax.y = bbmin.y = vert.y;
  for (int i = 1; i < size; ++i)
  {
    vert = m_poly[i];
    if (vert.x > bbmax.x) bbmax.x = vert.x;
    if (vert.y > bbmax.y) bbmax.y = vert.y;
    else if (vert.y < bbmin.y) bbmin.y = vert.y;
  }
  bbmin.x = bbmax.x * -1;
  bbmax.z = bbmax.x;
  bbmin.z = bbmax.z * -1;

  setBBmax(bbmax);
  setBBmin(bbmin);
}

// ********************************************
// copy constructor
DSLathe::DSLathe(const DSLathe &source) : DSsolid(source)
{
  m_poly = source.m_poly;
  m_slices = source.m_slices;
  m_rot = source.m_rot;
}

// ***********************************************
// destructor
DSLathe::~DSLathe()
{

}

// *****************************************************
// assignment op
DSLathe & DSLathe::operator =(const DSLathe &source)
{
  if (this != &source)
  {
    static_cast<DSsolid &>(*this) = source;        // call parent
    m_poly = source.m_poly;
    m_slices = source.m_slices;
    m_rot = source.m_rot;
  }
  return(*this);
}

// ************************************************************************
// vdraw
/* for performance, creating quad-strips (and tri-Fan) that wrap around
   the y-axis. Drawing with C and D leading A and B vertices around a 
   polygon edge.
*/
int DSLathe::vdraw(void)
{
  Vec3D vecA, vecB, vecC, vecD;          // for drawing a quad (tri)
  Vec3D backC, backD;                    // pre-transformation backup position
  Vec3D vec1, vec2, normal;              // for finding normal
  Vec3D backNorm;
  Mat3D rotation, backRotation;          // rotation matrix for lathing
  double angle;                          // angle of rotation
  int i = 0, j = 0;
  int size = m_poly.size();
  Vertex vert;
  int pt = 0;                            // holder for wrap around loop
  int triangle = -1;                     // check if a triangle

  // find rotation per slice
  if (m_rot == 1.0f && m_slices == 16)  angle = PI_OVER_8;       // some predefined
  else if (m_rot == 1.0f && m_slices == 8) angle = PI_OVER_4;
  else if (m_rot == 1.0f && m_slices == 4) angle = PI_OVER_2;
  else angle = (m_rot * TWO_PI) / m_slices;
  rotation = rot_y(angle);
  Mat3D backRot = rot_y(-angle);
  Mat3D temp = rotation;

  // start loop around edges of polygon
  for (i = 0; i < size; ++i) 
  {
    // prepare edge from poly
    vert = m_poly[i];
    vecA = Vec3D(vert.x, vert.y, vert.z);                                              
    if (i == (size - 1)) pt = 0;
    else pt = i + 1;
    vert = m_poly[pt];
    vecB = Vec3D(vert.x, vert.y, vert.z);      // B is next counterClockwise
    
    // if A is on the bottom, fan needs to rotate opposite
    if (vecA[0] == 0.0f) rotation = backRot;
    vecC = vecA * rotation;                     // initial step lathe
    vecD = vecB * rotation;

    // save backups for normal lathing positions before special translations
    // C and D are the only ones lathing on each step. A, B stay put
    backC = vecC;
    backD = vecD;

    // decide if this is a quad strip or tri-fan or nothing to draw loop
    triangle = -1;                           // reset
    if (vecC == vecA) triangle += 2;         // mark which one = 1
    if (vecD == vecB) triangle += 1;         //  = 0, or =2 for both

    // find normal pre-transformation
    vec1 = Vec3D(vecA[0] - vecB[0],
                 vecA[1] - vecB[1],
                 vecA[2] - vecB[2] );
    if (triangle == 1)
    {
      vec2 = Vec3D(vecD[0] - vecB[0],
                   vecD[1] - vecB[1],
                   vecD[2] - vecB[2] );
    }
    else
    {
      vec2 = Vec3D(vecC[0] - vecB[0],
                     vecC[1] - vecB[1],
                     vecC[2] - vecB[2] );
    }
    normal = vec1 * vec2;
    normal.unitNorm();
    backNorm = normal;             // save backup of pre-transformation
    
    // do special transformations
    if (m_rotation) 
    {
      vecA = vecA * *m_rotation;
      vecB = vecB * *m_rotation;
      vecC = vecC * *m_rotation;
      vecD = vecD * *m_rotation;
      normal = normal * *m_rotation;
    }
    if (m_translation) 
    {
      vecA = vecA + *m_translation;
      vecB = vecB + *m_translation;
      vecC = vecC + *m_translation;
      vecD = vecD + *m_translation;
      normal = normal + *m_translation;
    }

    if (triangle != 2)                       // else nothing to do 
    {
      if (triangle == -1) glBegin(GL_QUAD_STRIP);   // which to draw
      else glBegin(GL_TRIANGLE_FAN);

      
      // normal for starting verts
      glNormal3f(normal[0], normal[1], normal[2]);
      
      // start first edge
      if (triangle != 1)
      {
        glVertex3f(vecB[0], vecB[1], vecB[2]);
        glVertex3f(vecA[0], vecA[1], vecA[2]);
      }
      else
      {
        // fan must start at a point
        glVertex3f(vecA[0], vecA[1], vecA[2]);
        glVertex3f(vecB[0], vecB[1], vecB[2]);
      }

      // start loop around y-axis
      for (j = 0; j < m_slices; ++j)
      {
        // move normal for this next set
        normal = backNorm * rotation;
        backNorm = normal;                // update last
        if (m_rotation) normal = normal * *m_rotation;
        if (m_translation) normal += *m_translation;
        glNormal3f(normal[0], normal[1], normal[2]);

        if (triangle != 0) 
        {
          glVertex3f(vecD[0], vecD[1], vecD[2]);
          vecD = backD * rotation;                  // rotate for next seg
          backD = vecD;                             // save backup
          if (m_rotation) vecD = vecD * *m_rotation;
          if (m_translation) vecD += *m_translation;
        }
        if (triangle != 1) 
        {
          glVertex3f(vecC[0], vecC[1], vecC[2]);
          vecC = backC * rotation;                  // rotate for next seg
          backC = vecC;                             // save a backup
          if (m_rotation) vecC = vecC * *m_rotation;
          if (m_translation) vecC += *m_translation;
        }
      }
      // don't need to reconnect since we allow percentage turns
      glEnd();
    } // else nothing to do this i loop
    if (vecA[0] == 0.0f) rotation = temp;    // switch back   
  }
  return 1;
}