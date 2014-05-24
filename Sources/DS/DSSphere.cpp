// DSSphere.h
/*
    David Shrock
    CS447 - PSU
    2004.07.21
	
    implementation of DSSphere.
*/
#include "DSSphere.h"
#include <windows.h>					  // windows constants, required for gl.h
#include <GL/gl.h>						  // standard OpenGL include - for poly drawing
#include "../Vec3D.h"
#include "../Mat3D.h"
#include "../consts.h"          // for PI

// *************************************
// default constructor
DSSphere::DSSphere()
{
  Vertex bbmin, bbmax;
  m_radius = 1.0f;
  m_segs = 8;
  bbmin.x = bbmin.y = bbmin.z = -1.0f;
  bbmax.x = bbmax.y = bbmax.z = 1.0f;
  setBBmax(bbmax);
  setBBmin(bbmin);
}

// *****************************************
// primary constructor
DSSphere::DSSphere(float radius, int segs)
{
  Vertex bbmin, bbmax;
  if (radius > 0.00f) m_radius = radius;
  else m_radius = 1.0f;
  if (segs > 3) m_segs = segs;
  else m_segs = 8;

  // set bounding box
  bbmin.x = bbmin.y = bbmin.z = m_radius * -1;
  bbmax.x = bbmax.y = bbmax.z = m_radius;
  setBBmax(bbmax);
  setBBmin(bbmin);
}

// ********************************************
// copy constructor
DSSphere::DSSphere(const DSSphere &source) : DSsolid(source)
{
  m_radius = source.m_radius;
}

// ***********************************************
// destructor
DSSphere::~DSSphere()
{

}

// *****************************************************
// assignment op
DSSphere & DSSphere::operator =(const DSSphere &source)
{
  if (this != &source)
  {
    static_cast<DSsolid &>(*this) = source;        // call parent
    m_radius = source.m_radius;
  }
  return(*this);
}

// ************************************************************************
// vdraw

int DSSphere::vdraw(void)
{    
  Vec3D *vecs;                              // pts for sphere
  int i = 0, j = 0;
  int vIndex = 0;                           // which base vec on in loop
  Mat3D vertRot;                            // to position initial pts
  double angle;
  Vec3D vec1, vec2, norm;                   // to find surface normal
  int slices = m_segs * 2;
  int numVerts = slices * (m_segs - 1) + 2; 

  vecs = new Vec3D[numVerts];

  // find rotation per seg
  if (slices == 16)  angle = PI_OVER_8;       // some predefined
  else if (slices == 8) angle = PI_OVER_4;
  else angle = (TWO_PI) / slices;
  vertRot = rot_z(angle);

  // position points
  vecs[0] = Vec3D(0.0f, -m_radius, 0.0f);
  for (i = 0; i < m_segs; ++i)
  {
    vecs[i + 1] = vecs[i] * vertRot;         // remember vert(0) on bottom
  }                                          //   vert(m_segs) on top

  // rotate around y-axis to find remaining verts
  vertRot = rot_y(angle);
  
  for (i = (m_segs + 1); i < numVerts; ++i)
  {
    vecs[i] = vecs[i - m_segs] * vertRot;
  }

  // find out if any transformation
  if (m_translation)                       // later us 4D to combine both
  {
    for (i = 0; i < numVerts; ++i) 
    {
      vecs[i] += *m_translation;
    }
  }

  // draw bottom triangles which rotates down in index
  vIndex = (m_segs + 1) * slices;                   // position starting index
  glBegin(GL_TRIANGLES);

  for (i = 0; i < slices; ++i)
  {
    // find normal
  
    vec1 = Vec3D(vecs[vIndex][0] - vecs[0][0],
                 vecs[vIndex][1] - vecs[0][1],
                 vecs[vIndex][2] - vecs[0][2] );

    vec2 = Vec3D(vecs[vIndex - slices][0] - vecs[0][0],
                 vecs[vIndex - slices][1] - vecs[0][1],
                 vecs[vIndex - slices][2] - vecs[0][2] );  
    norm = vec1 * vec2;
    norm.unitNorm();
    glNormal3f(norm[0], norm[1], norm[2]);

    glVertex3f(vecs[0][0], vecs[0][1], vecs[0][2]);
    glVertex3f(vecs[vIndex][0], vecs[vIndex][1], vecs[vIndex][2]);
    glVertex3f(vecs[vIndex - slices][0], vecs[vIndex - slices][1],
               vecs[vIndex - slices][2]);

    vIndex -= m_segs;
  }
  glEnd();    // already reconnected, did full circle

  // drawing strips around from bottom to top
  for (i = 1; i < m_segs - 2; ++i)
  {
    glBegin(GL_LINES);
    // find first normal
    vec1 = Vec3D(vecs[i][0] - vecs[i + 1][0],
                 vecs[i][1] - vecs[i + 1][1],
                 vecs[i][2] - vecs[i + 1][2] );

    vec2 = Vec3D(vecs[i + m_segs][0] - vecs[i + 1][0],
                 vecs[i + m_segs][1] - vecs[i + 1][1],
                 vecs[i + m_segs][2] - vecs[i + 1][2] );  
    norm = vec1 * vec2;
    norm.unitNorm();
    glNormal3f(norm[0], norm[1], norm[2]);
    // first two points
    glVertex3f(vecs[i + 1][0], vecs[i + 1][1], vecs[i + 1][2]);
    glVertex3f(vecs[i][0], vecs[i][1], vecs[i][2]);

    for (j = 1; j < slices; ++j)
    {
      vIndex = i * (m_segs) + j;
      if (j != 1)    // already did first normal
      {
        vec1 = Vec3D(vecs[vIndex][0] - vecs[vIndex + 1][0],
                     vecs[vIndex][1] - vecs[vIndex + 1][1],
                     vecs[vIndex][2] - vecs[vIndex + 1][2] );

        vec2 = Vec3D(vecs[vIndex + m_segs][0] - vecs[vIndex + 1][0],
                     vecs[vIndex + m_segs][1] - vecs[vIndex + 1][1],
                     vecs[vIndex + m_segs][2] - vecs[vIndex + 1][2] );  
        norm = vec1 * vec2;
        norm.unitNorm();
        glNormal3f(norm[0], norm[1], norm[2]);
      }
      glVertex3f(vecs[vIndex + 1][0], vecs[vIndex + 1][1], vecs[vIndex + 1][2]);
      glVertex3f(vecs[vIndex][0], vecs[vIndex][1], vecs[vIndex][2]);
      //vIndex += m_segs;
    }
    glEnd();
  }
  // do top
  vIndex = m_segs - 1;
  glBegin(GL_TRIANGLES);
  for (i = 0; i < m_segs; ++i)
  {
    vec1 = Vec3D(vecs[vIndex][0] - vecs[m_segs][0],
                 vecs[vIndex][1] - vecs[m_segs][1],
                 vecs[vIndex][2] - vecs[m_segs][2] );

    vec2 = Vec3D(vecs[vIndex + m_segs][0] - vecs[m_segs][0],
                 vecs[vIndex + m_segs][1] - vecs[m_segs][1],
                 vecs[vIndex + m_segs][2] - vecs[m_segs][2] );  
    norm = vec1 * vec2;
    norm.unitNorm();
    glNormal3f(norm[0], norm[1], norm[2]);
    glVertex3f(vecs[m_segs][0], vecs[m_segs][1], vecs[m_segs][2]);
    glVertex3f(vecs[vIndex][0], vecs[vIndex][1], vecs[vIndex][2]);
    glVertex3f(vecs[m_segs][0], vecs[m_segs][1], vecs[m_segs][2]);
  }
  glEnd();
  delete [] vecs;
  return 1;
}