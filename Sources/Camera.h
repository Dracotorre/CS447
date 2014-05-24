// Camera.h
/*
  David Shrock
  CS447 - PSU
  2004.07.23

  header for Camera.cpp
  
  Camera could also be an Entity, but it's a little different. To simplify a bounding
  radius is used rather than a box, there are no draw, init, or destroy functions. Some
  of these might be useful for some game, but this isn't one of those.

  Camera coordinates are relative to whatever parent object it resides in (on). If held
  in a World, then world can use the lookAt vector returned by setCamDelta() directly 
  in gluLookAt(). However if some other object (like a player) is carrying the
  camera, then remember to translate the LookAt before using gluLookAt().

  Remember that when sending deltas to setCamDelta(), these are relative to the 
  Camera coords. Foreward along orientation vector is considered -z movement.

  For clarity: orientation is vector from camera position which could be called a look
      vector. lookAt is the resulting world vector from origin that cam is looking at.

*/

#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vec3D.h"

class Camera
{
  public:
    // constructor sets camera (0, 2, -5) with look vector <0,0,-1>
    Camera();
    // constructor to set position. pitch, yaw in radians
    Camera(Vec3D &position, float pitch, float yaw);   
    Camera(const Camera &);
    ~Camera();
    Camera & operator =(const Camera &);

    inline float getBoundRadius(void) { return (float)m_bSphere; }
    Vec3D & getPosition(void)  { return m_position; }
    Vec3D & getOrientation(void) { return m_orientation; } 
    Vec3D & getLookAt(void) { return m_lookAt; }

    // move camera to position and orientation; returns -1 for bad orientation
    int setCam(Vec3D &position, float pitch, float yaw);

    // adjust camera by vector position change and angle of orientation change
    // relative to Cam coords (facing -z is foreward movement of orientation)
    int setCamDelta(const Vec3D &deltaPos, float deltaPitch, float deltaYaw);

    // force camera to look at something
    int setLookAt(Vec3D &lookAt);



  private:
    double m_pitch;              //  pitch angle (x-axis rotation)
    double m_yaw;                //  yaw (y-axis rotation)
    Vec3D m_position;           // position relative to external parent object
    Vec3D m_orientation;        // view angle from m_position (normalized)
    Vec3D m_lookAt;             // vector relative to parent object
    //Vec3D m_velocity;

    scalarT m_bSphere;          // bounding radius for collisions

    inline void calcLookAt(void) { m_lookAt = m_position + 5 * m_orientation; }
    void calcOrientation(void);

};
#endif
