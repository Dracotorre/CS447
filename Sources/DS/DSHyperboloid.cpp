// DSHyperboloid.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.26
	
    implementation of DSHyperboloid extending DSsolid
*/
#include "DSHyperboloid.h"      // also includes Vec3D, Mat3D
#include <cmath>
using std::sqrt;
#include "../consts.h"          // for PI
#include <windows.h>            // windows constants, required for gl.h
#include <GL/gl.h>              // standard OpenGL include

// *************************************
// default constructor
DSHyperboloid::DSHyperboloid()
{
  Vertex bbmin, bbmax;            // to set bounding box
  m_a = 1.00f;
  m_b = 1.0f;
  m_ht = 1.0f;
  m_slices = 16;
  m_aSquare = 1.0;
  m_bSquare = 1.0;
  m_htSquare = 1.0;
  bbmin.x = bbmin.z = -1.41421356237f;  // square root(2)
  bbmax.x = bbmax.z = 1.41421356237f;
  bbmin.y = 0.0f;
  bbmax.y = 1.0f;
  setBBmax(bbmax);
  setBBmin(bbmin);
  m_numVerts = (HYPER_SEGS + 1) * m_slices;
  m_vecs = NULL;
}

// *************************************
// primary constructor
DSHyperboloid::DSHyperboloid(float baseRad, float topRad, float height, int slices)
{
  Vertex bbmin, bbmax;            // to set bounding box
  double max = 0.0;

  if (baseRad > 0.0f)
  {
    m_a = baseRad;
    m_aSquare = (double)m_a * m_a;
  }
  else
  {
    m_a = 1.0f;
    m_aSquare = 1.0;
  }
  if (height > 0.0f)
  {
    m_ht = height;
    m_htSquare = m_ht * m_ht;
  }
  else
  {
    m_ht = 1.0f;
    m_htSquare = 1.0;
  }
  if (topRad > m_a)
  {
    // calculate m_b and m_bSquare using equation of hyperbola
    m_bSquare = (double)(-m_htSquare /(1 - ((topRad * topRad) / m_aSquare)));
    m_b = (float)sqrt(m_bSquare);
  }
  else
  {
    m_b = 1.0f;
    m_bSquare = 1.0;
  }
  if (slices > 7) m_slices = slices;
  else slices = 16;

  bbmax.x = topRad;
  bbmax.z = topRad;
  bbmin.x = -topRad;
  bbmin.z = -topRad;
  bbmin.y = 0.0f;
  bbmax.y = m_ht;

  setBBmax(bbmax);
  setBBmin(bbmin);
  m_numVerts = (HYPER_SEGS + 1) * m_slices;
  m_vecs = NULL;
}

// *************************************
// copy constructor
DSHyperboloid::DSHyperboloid(const DSHyperboloid &source) : DSsolid(source)
{
  m_a = source.m_a;
  m_b = source.m_b;
  m_ht = source.m_ht;
  m_slices = source.m_slices;
  m_aSquare = source.m_aSquare;
  m_bSquare = source.m_bSquare;
  m_htSquare = source.m_htSquare;
  m_numVerts = source.m_numVerts;
  if (source.m_vecs)
  {
    m_vecs = new Vec3D[m_numVerts];
    for (int i = 0; i < m_numVerts; ++i)
    {
      m_vecs[i] = source.m_vecs[i];
    }
  }
  else m_vecs = NULL;
}

// *******************************
// destructor
DSHyperboloid::~DSHyperboloid()
{
  if (m_vecs) delete [] m_vecs;
}

// **********************************************
// assigment op
DSHyperboloid & DSHyperboloid::operator =(const DSHyperboloid &source)
{
  if (this != &source)
  {
    static_cast<DSsolid &>(*this) = source;        // call parent
    m_a = source.m_a;
    m_b = source.m_b;
    m_ht = source.m_ht;
    m_slices = source.m_slices;
    m_aSquare = source.m_aSquare;
    m_bSquare = source.m_bSquare;
    m_htSquare = source.m_htSquare;
    m_numVerts = source.m_numVerts;
    if (source.m_vecs)
    {
      m_vecs = new Vec3D[m_numVerts];
      for (int i = 0; i < m_numVerts; ++i)
      {
        m_vecs[i] = source.m_vecs[i];
      }
    }
    else m_vecs = NULL;
  }
  return (*this);
}

// **********************************************************************
// draw
/* Drawing vertices from bottom up to height using quad strips by lathing
   the hyperbola curve around the y-axis.
*/
int DSHyperboloid::vdraw(void)
{
  int i = 0, j = 0;
  int vIndex = 0;                         // which base vec on in loop
  int lastPts = 0;                        // to help find normals
  Vec3D vec1, vec2, norm;                 // to find surface normal
  int imod2 = 0;                          // to help flip/flop texture coords
  
  // must build each time in case of new transformations
  buildVerts();
  
  // draw each segment using strips
  for (j = 0; j < HYPER_SEGS; ++j)   
  {
    // Remember: 1 more pt than HYPER_SEGS to get number of segs right

    glBegin(GL_TRIANGLE_STRIP);

    // finding normals of the previous polygon surface to the points
    
    vec1 = Vec3D(m_vecs[m_numVerts - HYPER_SEGS - 1 + j][0] - 
                 m_vecs[m_numVerts - HYPER_SEGS - 1 + j + 1][0],
                 m_vecs[m_numVerts - HYPER_SEGS - 1 + j][1] - 
                 m_vecs[m_numVerts - HYPER_SEGS - 1 + j + 1][1],
                 m_vecs[m_numVerts - HYPER_SEGS - 1 + j][2] - 
                 m_vecs[m_numVerts - HYPER_SEGS - 1 + j + 1][2] );

    vec2 = Vec3D(m_vecs[j][0] - m_vecs[m_numVerts - HYPER_SEGS + j][0],
                 m_vecs[j][1] - m_vecs[m_numVerts - HYPER_SEGS + j][1],
                 m_vecs[j][2] - m_vecs[m_numVerts - HYPER_SEGS + j][2] );

    norm = vec1 * vec2;
    norm.unitNorm();
    glNormal3f(norm[0], norm[1], norm[2]);

    // first 2 points
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(m_vecs[j + 1][0], m_vecs[j + 1][1], m_vecs[j + 1][2]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(m_vecs[j][0], m_vecs[j][1], m_vecs[j][2]);

    for (i = 1; i < m_slices; ++i)    // 
    {
      vIndex = i * (HYPER_SEGS + 1) + j;
     // if (i != 1)             
     // {
        lastPts = vIndex - HYPER_SEGS - 1;
        vec1 = Vec3D(m_vecs[lastPts][0] - m_vecs[lastPts + 1][0],
                     m_vecs[lastPts][1] - m_vecs[lastPts + 1][1],
                     m_vecs[lastPts][2] - m_vecs[lastPts + 1][2] );

        vec2 = Vec3D(m_vecs[vIndex][0] - m_vecs[lastPts + 1][0],
                     m_vecs[vIndex][1] - m_vecs[lastPts + 1][1],
                     m_vecs[vIndex][2] - m_vecs[lastPts + 1][2] );  
        norm = vec1 * vec2;
        norm.unitNorm();
        glNormal3f(norm[0], norm[1], norm[2]);
     // }
      imod2 = i % 2;
      if (imod2 == 1) glTexCoord2f(1.0f, 0.0f);
      else glTexCoord2f(1.0f, 1.0f);
      glVertex3f(m_vecs[vIndex + 1][0], m_vecs[vIndex + 1][1], m_vecs[vIndex + 1][2]);
      if (imod2 == 1) glTexCoord2f(0.0f, 0.0f);
      else glTexCoord2f(0.0f, 1.0f);
      glVertex3f(m_vecs[vIndex][0], m_vecs[vIndex][1], m_vecs[vIndex][2]);
    }
    // reconnect to beginning
    vec1 = Vec3D(m_vecs[vIndex][0] - m_vecs[vIndex + 1][0],
                 m_vecs[vIndex][1] - m_vecs[vIndex + 1][1],
                 m_vecs[vIndex][2] - m_vecs[vIndex + 1][2] );

    vec2 = Vec3D(m_vecs[j][0] - m_vecs[vIndex + 1][0],
                 m_vecs[j][1] - m_vecs[vIndex + 1][1],
                 m_vecs[j][2] - m_vecs[vIndex + 1][2] );  
    norm = vec1 * vec2;
    norm.unitNorm();
    glNormal3f(norm[0], norm[1], norm[2]);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(m_vecs[j + 1][0], m_vecs[j + 1][1], m_vecs[j + 1][2]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(m_vecs[j][0], m_vecs[j][1], m_vecs[j][2]);

    glEnd();       
  }

  return 1;
}

// **********************************************************************
// private functions
// *******************************

// **********************************************
// buildVerts - place verts using hyperboloid function
void DSHyperboloid::buildVerts(void)
{
  Mat3D vertRot;                            // to position initial pts
  double angle;
  float xPos = 0.0f;                        // for calculating hyperbola
  float yPos = 0.0f;
  float htFactor = m_ht / HYPER_SEGS;       // for calculating yPos
  double yPosSquare = 0.0;
  int i = 0, j = 0;

  if (m_vecs) delete [] m_vecs;
  m_vecs = new Vec3D[m_numVerts];           // constructors set number

  // find rotation per slice
  if (m_slices == 16)  angle = PI_OVER_8;       // some predefined
  else if (m_slices == 8) angle = PI_OVER_4;
  else if (m_slices == 4) angle = PI_OVER_2;
  else angle = (TWO_PI) / m_slices;
  vertRot = rot_y(angle);

  // position first m_vecs along hyperbola on z=0 plane in +x
  m_vecs[0] = Vec3D(m_a, 0.0f, 0.0f);

  for (j = 1; j < HYPER_SEGS; ++j)    // don't do last since already have ht
  {
    yPos = (j * htFactor);
    yPosSquare = yPos * yPos;
    if (m_b == 1.0f) xPos = (float)sqrt(m_aSquare + m_aSquare * yPosSquare);
    else xPos = (float)sqrt(m_aSquare + yPosSquare * m_aSquare / m_bSquare);
    m_vecs[j] = Vec3D(xPos, yPos, 0.0f);
  }
  // now do last
  if (m_b == 1.0f) xPos = (float)sqrt(m_aSquare + m_aSquare * m_htSquare);
  else xPos = (float)sqrt(m_aSquare + m_htSquare * m_aSquare / m_bSquare);
  m_vecs[j] = Vec3D(xPos, m_ht, 0.0f);

  // rotate around y-axis to find remaining verts
  for (i = (HYPER_SEGS + 1); i < m_numVerts; ++i)
  {
    m_vecs[i] = m_vecs[i - HYPER_SEGS - 1] * vertRot;
  }

  // find out if we have any transformations
  if (m_rotation)
  {
    for (i = 0; i < m_numVerts; ++i)             
    {
      m_vecs[i] = m_vecs[i] * *m_rotation;
    }
  }
  if (m_translation)                       // later us 4D to combine both
  {
    for (i = 0; i < m_numVerts; ++i) 
    {
      m_vecs[i] += *m_translation;
    }
  }
  return;
}

