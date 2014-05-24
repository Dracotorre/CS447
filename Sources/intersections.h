// intersections.h
/*
    David Shrock
    CS447 - PSU
    2004.07.27
	
    header for intersections.cpp

    functions that test for intersections. 
*/
#ifndef _INTERSECTION_TESTS_H
#define _INTERSECTION_TESTS_H

const float INTSCT_TOL = 0.0004f;     // intersection tolerance

#include "DS/Poly.h"            // for Vertex      

/* aabbIntersection
   tests if two AABB intersect.
   returns: -1 for not, 1 for intersects  
*/
int aabbIntersection(Vertex &, Vertex &, Vertex &, Vertex &);

/* sphereAABBIntersect
   test if sphere intersects with AABB.
   send: sphere center, sphere radius, boxMax, boxMin
   returns: 1 for intersect, -1 for not
*/
int sphereAABBIntersect(const Vec3D &, float, const Vec3D &, const Vec3D &);

#endif