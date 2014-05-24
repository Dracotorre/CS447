// DSCylinder.h
/*
    David Shrock
    CS447 - PSU
    2004.07.27

    header for DSCylinder.cpp
    extends DSsolid

    Primitive open cylinder (or cone) Quadric. Top and bottom radii are 
    independant where the bottom may be 0 producing a cone. Cylinder 
    generated in +y direction on origin with open ends.

    Cylinder has texture coordinates that are sideways and repeat every
    slice. Good for noise maps. Be wary of stretching for tall cylinders.
    Cone does not contain texture coordinates.
*/
#ifndef _DS_CYLINDER_H
#define _DS_CYLINDER_H

#include "DSsolid.h"
#include "../Vec3D.h"

class DSCylinder : public DSsolid
{
  public:
    DSCylinder();
    DSCylinder(float topRad, float botRad, float height, int slices=16);
    DSCylinder(const DSCylinder &);
    ~DSCylinder();
    DSCylinder & operator =(const DSCylinder &);

    // draw the object
    int vdraw(void); 

  private:
    int     m_slices;          // number of segments around
    int     m_topNum;          // number vertices, may be different than slices
    int     m_botNum;
    float   m_bot;             // bottom radius
    float   m_top;             // top radius
    float   m_ht;              // height
    Vec3D  *m_bvecs;           // vertices on bottom
    Vec3D  *m_tvecs;           // vertices on top

    void buildVerts(void);     // puts vertices in position


    // inherited members: m_rotation, m_translation  -set by DSsolid
};
#endif
