// DSExtrude.h
/*
    David Shrock
    CS447 - PSU
    2004.07.19
	
    header for DSExtrude.cpp
    extends DSsolid

    A DSsolid that extrudes an arbitrary Poly in the -z direction. Given Poly
    must be not parallel to z plane, otherwise you'll get the default solid. 
    An untextured DSExtrude uses Quad Strips for performance, while textured
    type produces texture coordinates with Quads.

    If using textures: t repeats per segment while s wraps around polygon.
         Note that since grows in -z, if rotating up on y then textures will
         be upside down to preserve top-to-bottom in direction of growth.
*/
#ifndef _DS_EXTRUDE_H
#define _DS_EXTRUDE_H

#include "DSsolid.h"


class DSExtrude : public DSsolid
{
  public:
    // default constructor creates a round bar of length=1
    DSExtrude();  
    // primary constructor- send Poly, length, number of segments, texture(0, 1)
    DSExtrude(const Poly &, float len=1.0f, int segs=1, int texType = 0);
    DSExtrude(const DSExtrude &);
    ~DSExtrude();
    DSExtrude & operator =(const DSExtrude &);     // assignment op

    int vdraw(void);
    
  private:
    int   m_segs;                  // number of segments to draw
    int   m_texType;               // if texture coords =1, not =0
    float m_length;                // length of extrusion
    Poly  m_poly;                  // shape to extrude

    void renderNormal(void);       // draw without textures
    void renderTextured(void);     // draw with texture coords
};
#endif