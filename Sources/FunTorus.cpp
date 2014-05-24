// FunTorus.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.29

    implementation of FunTorus Entity.
*/
#include "FunTorus.h"
#include "materials.h"        // for red material
#include "consts.h"           // for PI

// default constructor
FunTorus::FunTorus()
{
  m_dList1= -1;

}

// copy constructor
FunTorus::FunTorus(const FunTorus &source)  : Entity(source)
{
  m_dList1 = -1;

}

// ***************************
// destructor
FunTorus::~FunTorus()
{

}

// **************************************************
// = operator overload
FunTorus & FunTorus::operator =(const FunTorus &source)
{
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent
    m_dList1 = -1;

  }
  return(*this);
}

// ***************************************************************
// destroy
// unload entity data from memory
void FunTorus::vdestroy(void)
{
  m_initialized = 0;

  return;
}

// ****************************************************************
// init
//
int FunTorus::vinit(void)
{

  Mat3D rotate = rot_x(PI_OVER_2);
  Vertex v1, v2, v3, v4;
  v1.x = 0.3f;
  v1.y = 0.0f;
  v2.x = 0.5f;
  v2.y = -0.12f;
  v3.x = 0.6f;
  v3.y = 0.0f;
  v4.x = 0.5f;
  v4.y = 0.12f;
  v1.z = v2.z = v3.z = v4.z = 0.0f;
  Poly polygon(v1, v2, v3, 4);
  polygon.addPoint(v4);
  DSLathe torus(polygon, 24);
  torus.setRotation(rotate);
  torus.setTranslation(Vec3D(0.0f, 0.6f, 0.0f));

  // generate list to capture drawing
  m_dList1=glGenLists(1);
  glNewList(m_dList1, GL_COMPILE);

  glPushMatrix();                    
    // set materials
    glMaterialfv(GL_FRONT, GL_AMBIENT, REDAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, REDDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, REDSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, REDShiny);;

    torus.vdraw();

  glPopMatrix();

  glEndList();
  
  // update bounding box for this Entity
  setBBox(Vec3D(0.60f, 2.0f, 0.50f), Vec3D(-0.60f, 0.0f, -0.50f));

  m_initialized = 1;

  return m_initialized;
}


// ***********************************************************
// render
// the toy constantly spins
void FunTorus::vrender(void)
{
  if (m_initialized == 1)
  {
    m_yaw += 0.4f;
    if (m_yaw >= 360.0f) m_yaw = 0.0f;
    glPushMatrix();
      glTranslatef(m_position[0], m_position[1], m_position[2]);
      glRotatef(m_yaw, 0.0f, 1.0f, 0.0f);
      glCallList(m_dList1);
    glPopMatrix();
  }
  return;
}
