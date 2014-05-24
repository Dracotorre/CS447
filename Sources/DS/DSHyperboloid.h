// DSHyperboloid.h
/*
    David Shrock
    CS447 - PSU
    2004.07.27
	
    header for DSHyperboloid.cpp
    extends DSsolid

    Primitive open Hyperboloid Quadric. Actually, the top half of a hyperboloid
    which can be flipped and joined to get a full. Can set base radius, top
    radius, and height. The top radius and the height will determine how
    much stretch (b) it has.

        Equation of hyperbola is: (x^2/a^2) - (y^2/b^2) = 1

    The default hyperboloid is radius of 1, height of 1, stretch of 1 and so the
    tap radius is square_root(2) at y=1;

    Included are texture coordinates sized per segment. Good for repeating textures
    such as noise maps or other small repating textures.
*/
#ifndef _DS_HYPERBOLOID_H
#define _DS_HYPERBOLOID_H

#include "DSsolid.h"

const int HYPER_SEGS = 4;                 // set number of vertical segments here

class DSHyperboloid : public DSsolid
{
  public:
    DSHyperboloid();
    DSHyperboloid(float baseRad, float topRad, float height, int slices=16);
    DSHyperboloid(const DSHyperboloid &);
    ~DSHyperboloid();
    DSHyperboloid & operator =(const DSHyperboloid &);

    // draw the object
    int vdraw(void); 

  private:
    int     m_slices;          // number of segments around
    int     m_numVerts;        // numver of total vertices
    float   m_b;               // stretch factor, calculated
    float   m_a;               // radius at base
    float   m_ht;              // height
    double  m_htSquare;        // store to save on recaculating
    double  m_aSquare;
    double  m_bSquare;
    Vec3D  *m_vecs;            // vertices

    void buildVerts(void);     // place vertices

    // inherited members: m_rotation, m_translation  -set by DSsolid
};
#endif
