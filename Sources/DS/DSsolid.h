// DSsolid.h
/*
    David Shrock
    CS447 - PSU
    2004.07.20
	
    header for DSsolid.cpp

    Base class of a solid object which may be used in cell decomposition. 
    This holds a Poly which may be supplied or else a default Poly (circle) 
    is generated. The bounding box (AABB) bounds the solid which must be 
    updated by children.

    For drawing, a caller may wish to avoid drawing any polygons that will 
    always be hidden (such as stacking another DSsolid). Use setHideFace() to 
    set a side to hide: 0= +z side, 1= +x, 2= -z, 3= -x, 4= +y, 5= -y which 
    will cause any polygons that are perfectly aligned with the bounding box 
    on that side not to draw. These sides correspond to pre-tranformations in 
    solid's original modeling location. Surface removal works best with
    Axis Aligned objects.
*/
#ifndef _DS_SOLID_H
#define _DS_SOLID_H

#include "Poly.h"               // for a polygon
#include "../Vec3D.h"
#include "../Mat3D.h"

class DSsolid
{
  public:
    DSsolid();                               // constructor
    DSsolid(const DSsolid &);                // copy constructor
    virtual ~DSsolid();                      // destructor
    DSsolid & operator =(const DSsolid &);   // assignment op

    // send Vertex holder for result. returns -1 if none to return else 1
    int getBBmax(Vertex &);
    int getBBmin(Vertex &);

    int setHideFace(int);                    // which bbox face not to draw (0-5)
    int setRotation(const Mat3D &);          // set translations from origin
    int setTranslation(const Vec3D &);          // not last position

    // draws object with OpenGL commands for requested solid
    virtual int vdraw(void);

  protected:
    short *m_hideFace;            // face of bbox not draw (+z,+x,-z,-x,+y,-y)
    Vec3D *m_translation;         // translation vector for each point
    Mat3D *m_rotation;            // rotation matrix of each point
       // considering altering to 4D matrix to hold both

    int drawPoly(Poly &polygon, int texType);
    int setBBmax(Vertex &);
    int setBBmin(Vertex &);

  private:
    Vertex *m_bbmax;              // bounding box mininum of solid
    Vertex *m_bbmin;              // bounding box maximum of solid
    Vertex *m_orig;               // original max and min

    void checkOrientation(void);  // checks bbmax and bbmin correctness
  
};
#endif
