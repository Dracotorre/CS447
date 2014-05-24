// DSSphere.h
/*
    David Shrock
    CS447 - PSU
    2004.07.21
	
    header for DSSphere.cpp
    extends DSsolid

    A sphere formed by triangles
*/
#ifndef _DS_SPHERE_H
#define _DS_SPHERE_H

#include "DSsolid.h"

class DSSphere : public DSsolid
{
  public:
    // default constructor, creates a torus
    DSSphere(); 
    // primary constructor- here segs is segments over a half-circle
    DSSphere(float radius, int segs=8);
    DSSphere(const DSSphere &);
    ~DSSphere();
    DSSphere & operator =(const DSSphere &);      // assignment op

    // draw the object
    int vdraw(void);                            

  private:
    int   m_segs;                      // level of detail
    float m_radius;                    // radius of sphere

    // inherited members: m_rotation, m_translation  -set by DSsolid
};
#endif
