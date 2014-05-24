// Camera.cpp
/*
  David Shrock
  CS447 - PSU
  2004.07.23

  implementation of Camera.
*/
#include "Camera.h"     // also includes Vec3D
#include "Mat3D.h"
#include "consts.h"

// ************************
// default constructor
// constructor sets camera (0, 2, -5) with look vector <0,0,-1>
Camera::Camera()
{
  m_pitch = 0.0f;
  m_yaw = 0.0f;
  m_position = Vec3D(0, 2, 5);
  m_orientation = Vec3D(0, 0, -1);
  m_lookAt = Vec3D(0, 2, 0);
  m_bSphere = 1.0f;
}

// *********************************************
// constructor to set position/orient
Camera::Camera(Vec3D &position, float pitch, float yaw)
{
  if (pitch > -PI_OVER_2 && pitch < PI_OVER_2) m_pitch = pitch;
  else m_pitch = 0.0f;
  if (yaw > -PI && yaw < PI) m_yaw = yaw;
  else m_yaw = 0.0f;
  m_position = position;
  if (pitch == 0.0f && yaw == 0.0f) m_orientation = Vec3D(0.0f, 0.0f, -1.0f);
  else calcOrientation();
  calcLookAt();                         // update m_lookAt
  m_bSphere = 0.5f;
}

// *************************************
// copy constructor
Camera::Camera(const Camera &source)
{
  m_pitch = source.m_pitch;
  m_yaw = source.m_yaw;
  m_position = source.m_position;
  m_orientation = source.m_orientation;
  m_lookAt = source.m_lookAt;
  m_bSphere = source.m_bSphere;
}

// ***************************
// destructor
Camera::~Camera()
{

}

// ********************************
// assignment op
Camera & Camera::operator =(const Camera &source)
{
  if (this != &source)
  {
    m_pitch = source.m_pitch;
    m_yaw = source.m_yaw; 
    m_position = source.m_position;
    m_orientation = source.m_orientation;
    m_lookAt = source.m_lookAt;
    m_bSphere = source.m_bSphere;
  }
  return (*this);
}

// ************************************************************
// jump camera to position and orientation
int Camera::setCam(Vec3D &position, float pitch, float yaw)
{
  m_position = position;
  if (pitch > -PI_OVER_2 && pitch < PI_OVER_2) m_pitch = pitch;
  else m_pitch = 0.0f;
  if (yaw > -PI && yaw < PI) m_yaw = yaw;
  else m_yaw = 0.0f;
  calcOrientation();
  calcLookAt();

  return 1;
}

// **********************************************************************
/* adjust camera by vector change and angle of orientation change
   returns resulting lookAt vector (for use with gluLookAt)
   if keeping track of velocity then calculate here.
   Remember: default orientation is polar- Theta(-90), phi(0)
*/
int Camera::setCamDelta(const Vec3D &deltaPos, float deltaPitch, float deltaYaw)
{
  // allow any change even if gigantic! -caller's in control
  int update = 0;
  Mat3D rotY = rot_y(m_yaw);
  Vec3D rotatedDelta = deltaPos * rotY;    // line up movment with our yaw

  m_position += rotatedDelta;

  if (deltaPitch != 0.0f && (deltaPitch + m_pitch) > -PI_OVER_2 && 
       (deltaPitch + m_pitch) < PI_OVER_2) 
  {
    m_pitch += deltaPitch;
    update = 1;
  }
  if (deltaYaw != 0.0f) 
  { 
    m_yaw += deltaYaw;
    if (m_yaw > TWO_PI) m_yaw -= TWO_PI;
    else if (m_yaw < -TWO_PI) m_yaw += TWO_PI;
    update = 1;
  }
  if (update == 1) calcOrientation();

  calcLookAt();
  return 1;
}

// *********************************************************************
// force camera to look at something
int Camera::setLookAt(Vec3D &lookAt)
{
  m_lookAt = lookAt;
  // must update orientation
  m_orientation = m_lookAt - m_position;
  m_orientation.unitNorm();

  // need to calculate pitch and yaw

  return 1;
}

// **********************************************************************
// private functions
// *******************


// ******************************************
// need to optimize this
void Camera::calcOrientation(void)
{
  Mat3D rotY = rot_y(m_yaw);
  Mat3D rotX = rot_x(m_pitch);
  Vec3D primary(0.0f, 0.0f, -1.0f);
  
  m_orientation = primary * rotX;
  m_orientation = m_orientation * rotY;
  return;
}
  
