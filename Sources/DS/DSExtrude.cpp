// DSExtrude.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.19
	
    implementation of DSExtrude.

    parent is DSsolid.

    A DSsolid that extrudes an arbitrary Poly in the -z direction. Given Poly
    must be in the z=0 plane.
*/
#include "DSExtrude.h"
#include <windows.h>					  // windows constants, required for gl.h
#include <GL/gl.h>						  // standard OpenGL include - for poly drawing

// *************************************
// default constructor
DSExtrude::DSExtrude()
{
  // default poly already set
  m_length = 1.0f;
  m_segs = 1;
  m_texType = 0;
}

// *****************************************
// primary constructor
DSExtrude::DSExtrude(const Poly &polygon, float len, int segs, int texType)
{
  // make sure poly not illegal - can't be parallel to z plane
  Vec3D normCheck(0.0f, 1.0f, 0.0f);
  Vec3D polyNorm;
  Vertex bbmin, bbmax;               // for bounding box 
  Vertex temp;                       // for collection from Poly
  int polySize = 0, i = 0;

  polyNorm = polygon.normal();
  if (polyNorm == normCheck || polyNorm == (normCheck * -1))
  {
    m_length = 1.0f;
    m_segs = 1;
    m_texType = 0;
  }
  else 
  {
    m_poly = polygon;
    if (len > 0.0f) m_length = len;
    else m_length = 1.0f;
    if (segs > 0) m_segs = segs;
    else m_segs = 1;   
    if (texType == 1) m_texType = 1;
    else texType = 0;
  }

  // find DSsolid bounding box
  polySize = m_poly.size();
  temp = m_poly[0];
  bbmin.x = bbmax.x = temp.x;           // init 
  bbmin.y = bbmax.y = temp.y;
  bbmin.z = bbmax.z = temp.z; 
  for (i = 1; i < polySize; ++i)
  {
    temp = m_poly[i];
    if (temp.x < bbmin.x) bbmin.x = temp.x;
    else if (temp.x > bbmax.x) bbmax.x = temp.x;
    if (temp.y < bbmin.y) bbmin.y = temp.y;
    else if (temp.y > bbmax.y) bbmax.y = temp.y;
    if (temp.z < bbmin.z) bbmin.z = temp.z;
    else if (temp.z > bbmax.z) bbmax.z = temp.z;
  }
  bbmax.z += m_length;

  setBBmax(bbmax);
  setBBmin(bbmin);
} 

// ********************************************
// copy constructor
DSExtrude::DSExtrude(const DSExtrude &source) : DSsolid(source)
{
  m_poly = source.m_poly;
  m_segs = source.m_segs;
  m_length = source.m_length;
  m_texType = source.m_texType;
  
}

// ***********************************************
// destructor
DSExtrude::~DSExtrude()
{

}

// *****************************************************
// assignment op
DSExtrude & DSExtrude::operator =(const DSExtrude &source)
{
  if (this != &source)
  {
    static_cast<DSsolid &>(*this) = source;        // call parent
    m_poly = source.m_poly;
    m_segs = source.m_segs;
    m_length = source.m_length;
    m_texType = source.m_texType;
  }
  return(*this);
}

// ************************************************************************
// vdraw
// for performance, creating quad-strips that wrap around each segment
// set A will be close side, B is far side (-z)
int DSExtrude::vdraw(void)
{
  if (m_texType == 1) renderTextured();
  else renderNormal();

  return 1;
}

// *************************************************************************
// private functions
// ***********************

// **************************************************************************
// renderNormal
void DSExtrude::renderNormal(void)
{
  int i = 0, j = 0;
  int polySize = m_poly.size();           // number vertices in Poly
  Vertex *vertsA;                         // trailing verts before translation
  Vertex *vertsB;                         // leading verts before trans
  Vec3D *vecsA;                           // translation copy of vertsA
  Vec3D *vecsB;                           // translation copy of vertsB
  float translate = -m_length / m_segs;   // translating in -z direction
  Vec3D vec1, vec2;                        // for finding normals
  Vec3D norm;
  float texFactor = (float)(1 / polySize); // for computing texture coords 
  Poly polyTemp;

  // initialize points
  vertsA = new Vertex[polySize];
  vertsB = new Vertex[polySize];
  vecsA = new Vec3D[polySize];
  vecsB = new Vec3D[polySize];
  for (i = 0; i < polySize; ++i)
  {
    vertsA[i] = m_poly[i];                // verts stay in base locations
    vertsB[i] = m_poly[i];
    vertsB[i].z += translate;
    vecsA[i] = Vec3D(vertsA[i].x, vertsA[i].y, vertsA[i].z);
    vecsB[i] = Vec3D(vertsB[i].x, vertsB[i].y, vertsB[i].z);
    if (m_rotation)
    {
      vecsA[i] = vecsA[i] * *m_rotation;  // vecs get moved for drawing
      vecsB[i] = vecsB[i] * *m_rotation;
    }
    if (m_translation)
    {
      vecsA[i] = vecsA[i] + *m_translation;
      vecsB[i] = vecsA[i] + *m_translation;
    }
  }
   
  // draw first end polygon
  if (m_hideFace)
  {
    if (m_hideFace[0] == 0)                // make sure not hidden
    {
      polyTemp = m_poly;
      if (m_rotation) polyTemp.rot(*m_rotation);
      if (m_translation) polyTemp.translate(*m_translation);
      drawPoly(polyTemp, m_texType);
    }
  }

  // start segment loop
  for (j = 0; j < m_segs; ++j)
  {
    // find normal of first quad
    vec1 = Vec3D(vecsB[0][0] - vecsA[0][0],
                 vecsB[0][1] - vecsA[0][1],
                 vecsB[0][2] - vecsA[0][2] );

    vec2 = Vec3D(vecsB[1][0] - vecsA[0][0],
                 vecsB[1][1] - vecsA[0][1],
                 vecsB[1][2] - vecsA[0][2] );
    norm = vec1 * vec2;
    norm.unitNorm();

    glBegin(GL_QUAD_STRIP);
    glNormal3f(norm[0], norm[1], norm[2]);
    // do first 2 of first quad
    glVertex3f(vecsA[0][0], vecsA[0][1], vecsA[0][2]);
    glVertex3f(vecsB[0][0], vecsB[0][1], vecsB[0][2]);
    for (i = 1; i < polySize; ++i)
    {
      if (i != 1)
      {
        // find normal of this quad
        vec1 = Vec3D(vecsB[i - 1][0] - vecsA[i - 1][0],
                     vecsB[i - 1][1] - vecsA[i - 1][1],
                     vecsB[i - 1][2] - vecsA[i - 1][2] );
        vec2 = Vec3D(vecsB[i][0] - vecsA[i - 1][0],
                     vecsB[i][1] - vecsA[i - 1][1],
                     vecsB[i][2] - vecsA[i - 1][2] );
        norm = vec1 * vec2;
        norm.unitNorm();
        glNormal3f(norm[0], norm[1], norm[2]);
      }
      glVertex3f(vecsA[i][0], vecsA[i][1], vecsA[i][2]);
      glVertex3f(vecsB[i][0], vecsB[i][1], vecsB[i][2]);
    }
    // connect back to origin - last quad in strip
    glVertex3f(vecsA[0][0], vecsA[0][1], vecsA[0][2]);
    glVertex3f(vecsB[0][0], vecsB[0][1], vecsB[0][2]);
    glEnd();

    // set up for next segment if any
    if ((j + 1) != m_segs)
    {
      for (i = 0; i < polySize; ++i)
      {
        // do extrusion with the base
        // catch A up with B                 
        vertsA[i].z = vertsB[i].z;
        // translate B
        vertsB[i].z += translate;
        // copy and do translations for drawing
        vecsA[i] = Vec3D(vertsA[i].x, vertsA[i].y, vertsA[i].z);
        vecsB[i] = Vec3D(vertsB[i].x, vertsB[i].y, vertsB[i].z);
        if (m_rotation)
        {
          vecsA[i] = vecsA[i] * *m_rotation;
          vecsB[i] = vecsB[i] * *m_rotation;
        }
        if (m_translation)
        {
          vecsA[i] += *m_translation;
          vecsB[i] += *m_translation;
        }
      }
    }
  }
  // draw back end Poly
  if (m_hideFace)
  {
    if (m_hideFace[2] == 0)                // make sure not hidden
    {
      Poly back(vertsB[polySize -1], vertsB[polySize -2], vertsB[polySize -3]);
      for (i = polySize -4; i >= 0; --i)
      {
        back.addPoint(vertsB[i]);
      }
      if (m_rotation) back.rot(*m_rotation);
      if (m_translation) back.translate(*m_translation);
      drawPoly(back, m_texType);
    }
  }
 
  if (vertsA) delete [] vertsA;
  if (vertsB) delete [] vertsB;
  if (vecsA) delete [] vecsA;
  if (vecsB) delete [] vecsB;
  return;
}

// **************************************************************************
// renderTextured
void DSExtrude::renderTextured(void)
{

  return;
}