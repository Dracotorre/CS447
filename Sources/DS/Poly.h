// Poly.h
/*
    David Shrock
    CS447 - PSU
    2004.07.18
	
    header for Poly.cpp

    a polygon containing only points. 
    The default constructor produces a circle facing +z of radius 1.
    Use the primary constructor to start a polygon of 3 points along with an 
    optional planned size for efficient building. When adding points, they should
    be in order of counter-clockwise. Any non-planar points added are rejected along
    with duplicates.
    Use the [] operator to get a vertex.

    Data structure to hold vertices in Poly is a dynamic array.
*/
#ifndef _DS_POLY_H_           
#define _DS_POLY_H_

#include "../Vec3D.h"                  // for deciding if planar and normal
#include "../Mat3D.h"
#include <cstddef>                     // for NULL
//#include "VertList.h"        // vertex list for easy passing
//using namespace ds_verts;

struct Vertex       // for easy passing around
{
  float x;
  float y;
  float z;
};

class Poly
{
  public:
    Poly();                 // default constructor, makes a circle

    // primary constructor
    // send 3 vertices to start, total planned points. Ok not to fill.
    // if 3 vertices contain a duplicate then get a circle instead.
    Poly(const Vertex &, const Vertex &, const Vertex &, int total=3);  

    Poly(const Poly &);     // copy constructor
    ~Poly();                // destructor
    Poly & operator =(const Poly &);

    // returns vertex of requested index
    Vertex & operator [](int index) const; 

    // add another point in order. returns -1 if point is bad- not added
    int addPoint(const Vertex &);
    int addPoint(const float, const float, const float);

    // return normal
    Vec3D normal(void) const { return m_normal; }

    // rotate entire Poly
    int rot(const Mat3D &);

    // translate entire Poly in direction of vector
    int translate(const Vec3D &);

    // return number of vertices in array
    int size(void) { return m_size; }   

  private:
    int     m_size;            // number of m_verts
    int     m_maxSize;         // array size, if add more then recreate array
    int     m_onZ;             // if planar with z axis
    Vec3D   m_normal;          // normal of the polygon
    Vertex *m_verts;           // vertex array
    

    int addToPoly(float, float, float);   // add to end of vertex list
    int checkDupes(float, float, float);  // 
    int checkPlanar(float, float, float); // if planar with poly
    void circle(void);                    // creates the default circle

};
#endif
