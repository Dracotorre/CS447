// DSCylinder.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.27

    implementation of DSCylinder extending DSsolid
*/
#include "DSCylinder.h"         // also includes Vec3D, Mat3D
#include "../consts.h"          // for PI
#include <windows.h>            // windows constants, required for gl.h
#include <GL/gl.h>              // standard OpenGL include - for poly drawing

// *************************************
// default constructor
DSCylinder::DSCylinder()
{
  Vertex bbmin, bbmax;            // to set bounding box
  m_bot = 0.25f;
  m_top = 0.25f;
  m_ht = 3.0f;
  m_slices = 16;
  bbmin.x = bbmin.z = -0.25f;
  bbmax.x = bbmax.z = 0.25f;
  bbmin.y = 0.0f;
  bbmax.y = 3.0f;
  setBBmax(bbmax);
  setBBmin(bbmin);
  m_botNum = 0;
  m_topNum = 0;
  m_bvecs = NULL;
  m_tvecs = NULL;
}

// *************************************
// primary constructor
DSCylinder::DSCylinder(float topRad, float botRad, float height, int slices)
{
  Vertex bbmin, bbmax;            // to set bounding box

  if (topRad > 0.0f) m_top = topRad;    // don't allow top to be 0.0f
  else m_top = 0.25f;
  if (botRad >= 0.0f) m_bot = botRad;
  else m_bot = 0.25f;
  if (height > 0.0f) m_ht = height;
  else m_ht = 3.0f;
  if (slices > 7) m_slices = slices;
  else m_slices = 16;

  if (m_top > m_bot) 
  {
    bbmin.x = bbmin.z = -1 * m_bot;
    bbmax.x = bbmax.z = m_top;
  }
  else
  {
    bbmin.x = bbmin.z = -1 * m_top;
    bbmax.x = bbmax.z = m_bot;
  }
  bbmax.y = m_ht;
  bbmin.y = 0.0f;

  setBBmax(bbmax);
  setBBmin(bbmin);

  if (m_bot > 0.0f) m_botNum = m_slices;
  else m_botNum = 0;
  m_topNum = m_slices;
  m_bvecs = NULL;
  m_tvecs = NULL;
}

// *************************************
// copy constructor
DSCylinder::DSCylinder(const DSCylinder &source) : DSsolid(source)
{
  int i = 0;
  m_bot = source.m_bot;
  m_top = source.m_top;
  m_ht = source.m_ht;
  m_slices = source.m_slices;
  m_topNum = source.m_topNum;
  m_botNum = source.m_botNum;

  if (source.m_bvecs)
  {
    m_bvecs = new Vec3D[m_botNum];
    for (i = 0; i < m_botNum; ++i)
    {
      m_bvecs[i] = source.m_bvecs[i];
    }
  }
  else m_bvecs = NULL;
  if (source.m_tvecs)
  {
    m_tvecs = new Vec3D[m_topNum];
    for (i = 0; i < m_topNum; ++i)
    {
      m_tvecs[i] = source.m_tvecs[i];
    }
  }
  else m_tvecs = NULL;
}

// *******************************
// destructor
DSCylinder::~DSCylinder()
{
  if (m_bvecs) delete [] m_bvecs;
  if (m_tvecs) delete [] m_tvecs;
}

// **********************************************
// assigment op
DSCylinder & DSCylinder::operator =(const DSCylinder &source)
{
  int i = 0;
  if (this != &source)
  {
    static_cast<DSsolid &>(*this) = source;        // call parent
    m_bot = source.m_bot;
    m_top = source.m_top;
    m_ht = source.m_ht;
    m_slices = source.m_slices;
    m_botNum = source.m_botNum;
    m_topNum = source.m_topNum;
    if (source.m_bvecs)
    {
      m_bvecs = new Vec3D[m_botNum];
      for (i = 0; i < m_botNum; ++i)
      {
        m_bvecs[i] = source.m_bvecs[i];
      }
    }
    else m_bvecs = NULL;
    if (source.m_tvecs)
    {
      m_tvecs = new Vec3D[m_topNum];
      for (i = 0; i < m_topNum; ++i)
      {
        m_tvecs[i] = source.m_tvecs[i];
      }
    }
    else m_tvecs = NULL;
  }
  return (*this);
}

// ****************************************************************
// draw the object
// for textures, drawing sidways repeating
int DSCylinder::vdraw(void)
{
  int i = 0;
  int imod2 = 0;
  Vec3D vec1, vec2, norm;                   // to find surface normal

  // always build new verts since transformations can change
  buildVerts();

  if (m_botNum == 1)
  {
    glBegin(GL_TRIANGLE_FAN);
    // upside-down, so need to draw circling backwards
    // find first normal
    vec1 = Vec3D(m_bvecs[0][0] - m_tvecs[m_topNum - 1][0],
                 m_bvecs[0][1] - m_tvecs[m_topNum - 1][1],
                 m_bvecs[0][2] - m_tvecs[m_topNum - 1][2] );

    vec2 = Vec3D(m_tvecs[0][0] - m_tvecs[m_topNum - 1][0],
                 m_tvecs[0][1] - m_tvecs[m_topNum - 1][1],
                 m_tvecs[0][2] - m_tvecs[m_topNum - 1][2] );
    norm = vec1 * vec2;
    norm.unitNorm();
    glNormal3f(norm[0], norm[1], norm[2]);

    // first pair of first tri
    glVertex3f(m_bvecs[0][0], m_bvecs[0][1], m_bvecs[0][2]);
    glVertex3f(m_tvecs[0][0], m_tvecs[0][1], m_tvecs[0][2]);

    // circle around back doing pt for each tri-fan
    for (i = m_topNum - 1; i > 0; --i)
    {
      if (i != m_topNum - 1)     // already calc first one
      {
        vec1 = Vec3D(m_bvecs[0][0] - m_tvecs[i - 1][0],
                     m_bvecs[0][1] - m_tvecs[i - 1][1],
                     m_bvecs[0][2] - m_tvecs[i - 1][2] );

        vec2 = Vec3D(m_tvecs[i][0] - m_tvecs[i - 1][0],
                     m_tvecs[i][1] - m_tvecs[i - 1][1],
                     m_tvecs[i][2] - m_tvecs[i - 1][2] );
        norm = vec1 * vec2;
        norm.unitNorm();
        glNormal3f(norm[0], norm[1], norm[2]);
      }

      glVertex3f(m_tvecs[i][0], m_tvecs[i][1], m_tvecs[i][2]);
    }
    // reconnect
    vec1 = Vec3D(m_bvecs[0][0] - m_tvecs[0][0],
                 m_bvecs[0][1] - m_tvecs[0][1],
                 m_bvecs[0][2] - m_tvecs[0][2] );

    vec2 = Vec3D(m_tvecs[1][0] - m_tvecs[0][0],
                 m_tvecs[1][1] - m_tvecs[0][1],
                 m_tvecs[1][2] - m_tvecs[0][2] );
    norm = vec1 * vec2;
    norm.unitNorm();
    glNormal3f(norm[0], norm[1], norm[2]);

    glVertex3f(m_tvecs[0][0], m_tvecs[0][1], m_tvecs[0][2]);

  }
  else
  {
    // normal cylinder - doing triangle strips rather than quads
    glBegin(GL_TRIANGLE_STRIP);
    
    // finding normals of the previous polygon surface to the points

    vec1 = Vec3D(m_bvecs[m_topNum - 1][0] - m_tvecs[m_topNum - 1][0],
                 m_bvecs[m_topNum - 1][1] - m_tvecs[m_topNum - 1][1],
                 m_bvecs[m_topNum - 1][2] - m_tvecs[m_topNum - 1][2] );

    vec2 = Vec3D(m_bvecs[0][0] - m_tvecs[m_topNum - 1][0],
                 m_bvecs[0][1] - m_tvecs[m_topNum - 1][1],
                 m_bvecs[0][2] - m_tvecs[m_topNum - 1][2] );
    norm = vec1 * vec2;
    norm.unitNorm();
    glNormal3f(norm[0], norm[1], norm[2]);

    // do first pair
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(m_tvecs[0][0], m_tvecs[0][1], m_tvecs[0][2]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(m_bvecs[0][0], m_bvecs[0][1], m_bvecs[0][2]);

    for (i = 1; i < m_slices; ++i)
    {
      imod2 = i % 2;
      // find normal - alternatively could transform last loop normal
      vec1 = Vec3D(m_bvecs[i - 1][0] - m_tvecs[i - 1][0],
                   m_bvecs[i - 1][1] - m_tvecs[i - 1][1],
                   m_bvecs[i - 1][2] - m_tvecs[i - 1][2] );

      vec2 = Vec3D(m_bvecs[i][0] - m_tvecs[i - 1][0],
                   m_bvecs[i][1] - m_tvecs[i - 1][1],
                   m_bvecs[i][2] - m_tvecs[i - 1][2] );
      norm = vec1 * vec2;
      norm.unitNorm();
      glNormal3f(norm[0], norm[1], norm[2]);

      if (imod2 == 1) glTexCoord2f(0.0f, 1.0f);
      else glTexCoord2f(1.0f, 1.0f);
      glVertex3f(m_tvecs[i][0], m_tvecs[i][1], m_tvecs[i][2]);
      if (imod2 == 1) glTexCoord2f(0.0f, 0.0f);
      else glTexCoord2f(1.0f, 0.0f);
      glVertex3f(m_bvecs[i][0], m_bvecs[i][1], m_bvecs[i][2]);
    }
    // reconnect
    vec1 = Vec3D(m_bvecs[m_botNum - 1][0] - m_tvecs[m_topNum - 1][0],
                 m_bvecs[m_botNum - 1][1] - m_tvecs[m_topNum - 1][1],
                 m_bvecs[m_botNum - 1][2] - m_tvecs[m_topNum - 1][2] );

    vec2 = Vec3D(m_bvecs[0][0] - m_tvecs[m_topNum - 1][0],
                 m_bvecs[0][1] - m_tvecs[m_topNum - 1][1],
                 m_bvecs[0][2] - m_tvecs[m_topNum - 1][2] );
    norm = vec1 * vec2;
    norm.unitNorm();
    glNormal3f(norm[0], norm[1], norm[2]);
    
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(m_tvecs[0][0], m_tvecs[0][1], m_tvecs[0][2]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(m_bvecs[0][0], m_bvecs[0][1], m_bvecs[0][2]);
  }
  glEnd();

  return 1;
}

// ***********************************************************************
// private functions
// *********************

// ****************************************************8
// buildVerts
void DSCylinder::buildVerts(void)
{
  Mat3D vertRot;                            // to position initial pts
  double angle = 0.0;
  int i = 0;

  // find rotation per slice
  if (m_slices == 16)  angle = PI_OVER_8;       // some predefined
  else if (m_slices == 8) angle = PI_OVER_4;
  else if (m_slices == 4) angle = PI_OVER_2;
  else angle = (TWO_PI) / m_slices;
  vertRot = rot_y(angle);

  if (m_bvecs) delete [] m_bvecs;
  if (m_tvecs) delete [] m_tvecs;

  // set number of vertices and position them
  if (m_bot == 0.0f)
  {
    m_bvecs = new Vec3D[1];
    m_bvecs[0] = Vec3D(0.0f, 0.0f, 0.0f);
  }
  else
  {
    m_bvecs = new Vec3D[m_botNum];
    m_bvecs[0] = Vec3D(0.0f, 0.0f, m_bot);
    for (i = 1; i < m_botNum; ++i)
    {
      m_bvecs[i] = m_bvecs[i - 1] * vertRot;
    }
  }
  m_tvecs = new Vec3D[m_topNum];
  m_tvecs[0] = Vec3D(0.0f, m_ht, m_top);
  for (i = 1; i < m_topNum; ++i)
  {
    m_tvecs[i] = m_tvecs[i - 1] * vertRot;
  }

  // do any post transformations if any
  if (m_rotation)
  {
    for (i = 0; i < m_botNum; ++i)
    {
      m_bvecs[i] = m_bvecs[i] * *m_rotation;
    }
    for (i = 0; i < m_topNum; ++i)
    {
      m_tvecs[i] = m_tvecs[i] * *m_rotation;
    }
  }
  if (m_translation)
  {
    for (i = 0; i < m_botNum; ++i)
    {
      m_bvecs[i] += *m_translation;
    }
    for (i = 0; i < m_topNum; ++i)
    {
      m_tvecs[i] += *m_translation;
    }
  }

  return;
}


