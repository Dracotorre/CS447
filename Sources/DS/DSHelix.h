// DSHelix.h
/*
    David Shrock
    CS447 - PSU
    2004.07.21
	
    header for DSHelix.cpp
    extends DSsolid

    A helix made of circle to form a "coil spring."
    In, future may add other shapes such an arbitrary polygon as the base shape
    for the Helix.

    vector equation of Helix: r(t) = 2cos(ti) + tk + sin(tj)
*/
#ifndef _DS_HELIX_H
#define _DS_HELIX_H

#include "DSsolid.h"

const int HELIX_POLY_VERTS = 6;       // num vertices for circle

class DSHelix : public DSsolid
{
  public:
    // default constructor, creates a torus
    DSHelix(); 
    // primary constructor- send radius
    DSHelix(float radius, float circleRad=0.1f, int coils=4, float ht=1.0f);
    DSHelix(const DSHelix &);
    ~DSHelix();
    DSHelix & operator =(const DSHelix &);      // assignment op

    // draw the object
    int vdraw(void);                            

  private:
    int    m_numVerts;                // number of vertices
    int    m_coils;                   // number of times to wrap
    float  m_ht;                      // height of helix
    float  m_majRadius;               // radius of cylinder helix wraps around
    float  m_minRadius;               // radius of circle
    Vec3D *m_verts;                   // vertices

    void buildVerts(void);            // place the vertices
    

    // inherited members: m_rotation, m_translation  -set by DSsolid
};
#endif