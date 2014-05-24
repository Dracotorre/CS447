// DSLathe.h
/*
    David Shrock
    CS447 - PSU
    2004.07.19
	
    header for DSLathe.cpp
    extends DSsolid

    Lathes an arbitrary Poly around the y-axis. Poly should only reside on the z=0 
    plane completely on the +x right-side. Points may reside on y=0 to get a closed 
    circular DSsolid.
*/
#ifndef _DS_LATHE_H
#define _DS_LATHE_H

#include "DSsolid.h"

class DSLathe : public DSsolid
{
  public:
    // default constructor, creates a torus
    DSLathe(); 
    // primary constructor- send Poly, slices, percentage of rotation
    DSLathe(const Poly &, int slices=24, float percent=1.0);
    DSLathe(const DSLathe &);
    ~DSLathe();
    DSLathe & operator =(const DSLathe &);      // assignment op

    // draw the object
    int vdraw(void);                            

  private:
    int   m_slices;                    // number of segments around
    float m_rot;                       // percentage of rotation
    Poly  m_poly;                      // the base Poly to rotate

    // inherited members: m_rotation, m_translation  -set by DSsolid
};
#endif
