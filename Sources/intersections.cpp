// intersections.h
/*
    David Shrock
    CS447 - PSU
    2004.07.27
	
    implementation of intersection test functions

    References:
    For algorithms used. Complete list of references may be found in 
    overview.doc.

      Akenine-Moller, Tomas and Hains, Eric. "Real-Time Rendering 2nd Ed"
      2002, AK Peters.
*/
#include "intersections.h"

// *************************************
// aabbIntersection
/* test if two AABB intersect.
   returns: -1 for not, 0 for aligned face, 1 for intersects 
*/
int aabbIntersection(Vertex &aMax, Vertex &aMin, Vertex &bMax, Vertex &bMin)
{
  int result = 0;

  if ( (aMin.x + INTSCT_TOL > bMax.x || bMin.x + INTSCT_TOL > aMax.x) ||
       (aMin.y + INTSCT_TOL > bMax.y || bMin.y + INTSCT_TOL > aMax.y) ||
       (aMin.z + INTSCT_TOL > bMax.z || bMin.z + INTSCT_TOL > aMax.z) )
  {
    result = -1;
  }
  else result = 1;

  return result;
} 

// *************************************************************************
/* sphereAABBIntersect
   test if sphere intersects with AABB.
   based on Algorithm by Arvo (Akenine-Moller 599): find AABB point that
   is closest to sphere and see if it is inside radius.
   returns: 1 for intersect, -1 for not
*/
int sphereAABBIntersect(const Vec3D &center, float radius, const Vec3D &bMax,
                        const Vec3D &bMin)
{
  int i = 0;
  int result = 0;
  float sumDistance = 0.0f;                   // init before loop
  float distSquare = 0.0f;

  for (i = 0; i < 3; ++i)                     // for each x,y,z
  {
    if (center[i] < bMin[i]) 
    {
      distSquare = (center[i] - bMin[i]) * (center[i] - bMin[i]);
      sumDistance += distSquare;
    }
    else if (center[i] > bMax[i])
    {
      distSquare = (center[i] - bMax[i]) * (center[i] - bMax[i]);
      sumDistance += distSquare;
    }
  }
  // if sum of distances is less than radius square, then overlaps
  if (sumDistance > (radius * radius)) result = -1;
  else result = 1;

  return result;
}