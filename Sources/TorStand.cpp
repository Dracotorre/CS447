// TorStand.cpp
/*
    David Shrock
    CS447 - PSU
    2004.08.01

    implementation of TorStand Entity.
*/
#include "TorStand.h"
#include "materials.h"        // for gold material
#include "consts.h"           // for PI
#include "DS/DSCells.h"
using namespace ds_cells;

// default constructor
TorStand::TorStand()
{
  m_dList1= -1;
  m_top = NULL;
  m_bot = NULL;
  m_torus = NULL;
}

// copy constructor
TorStand::TorStand(const TorStand &source)  : Entity(source)
{
  m_dList1 = source.m_dList1;
  if (source.m_top)
  {
    m_top = new DSHyperboloid;
    *m_top = *source.m_top;
  }
  else m_top = NULL;
  if (source.m_bot)
  {
    m_bot = new DSHyperboloid;
    *m_bot = *source.m_bot;
  }
  else m_bot = NULL;
  if (source.m_torus)
  {
    m_torus = new FunTorus;
    *m_torus = *source.m_torus;
  }
  else m_torus = NULL;
}


// ***************************
// destructor
TorStand::~TorStand()
{
  if (m_top) delete m_top;
  if (m_bot) delete m_bot;
  if (m_torus) delete m_torus;
}

// **************************************************
// = operator overload
TorStand & TorStand::operator =(const TorStand &source)
{
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent
    m_dList1 = source.m_dList1;
    if (source.m_top)
    {
      m_top = new DSHyperboloid;
      *m_top = *source.m_top;
    }
    else m_top = NULL;
    if (source.m_bot)
    {
      m_bot = new DSHyperboloid;
      *m_bot = *source.m_bot;
    }
    else m_bot = NULL;
    if (source.m_torus)
    {
      m_torus = new FunTorus;
      *m_torus = *source.m_torus;
    }
    else m_torus = NULL;
  }
  return(*this);
}

// ***************************************************************
// destroy
// unload entity data from memory
void TorStand::vdestroy(void)
{
  m_initialized = 0;

  return;
}

// ****************************************************************
// init
//
int TorStand::vinit(void)
{
  Mat3D rot = rot_x(PI);
  DSCells stack;

  if (!m_bot)
  {
    m_bot = new DSHyperboloid(0.1f, 0.333f, 0.5f);
    m_bot->setRotation(rot);
    m_bot->setTranslation(Vec3D(0.0f, 0.5f, 0.0f));
  }
  if (!m_top)
  {
    m_top = new DSHyperboloid(0.1f, 0.2f, 0.5f);
    m_top->setTranslation(Vec3D(0.0f, 0.5f, 0.0f));
  }
  stack.add(*m_bot);
  stack.add(*m_top);

  if (m_torus) delete m_torus;
  m_torus = new FunTorus();
  m_torus->vinit();
  m_torus->setPosition(Vec3D(0.0f, 0.42f, 0.0f));


  rot = rot_y(PI / 8);               // set for drawing cap
  Vec3D pt(0.2f, 1.0f, 0.0f);

  // generate list to capture drawing
  m_dList1=glGenLists(1);
  glNewList(m_dList1, GL_COMPILE);

  glPushMatrix();
    // set materials
    glMaterialfv(GL_FRONT, GL_AMBIENT, SHINY_GOLDAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SHINY_GOLDDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SHINY_GOLDSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, SHINY_GOLDShiny);;

    stack.draw();

    // put a cap on the top
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.1f, 0.0f);
    glVertex3f(pt[0], pt[1], pt[2]);
    for (int i = 1; i < 16; ++i)
    {
      pt = pt * rot;
      glVertex3f(pt[0], pt[1], pt[2]);
    }
    glEnd();

  glPopMatrix();

  glEndList();
  
  // update bounding box for this Entity
  setBBox(Vec3D(0.50f, 3.0f, 0.50f), Vec3D(-0.50f, 0.0f, -0.50f));

  m_initialized = 1;

  return m_initialized;
}


// ***********************************************************
// render
// the toy constantly spins
void TorStand::vrender(void)
{
  if (m_initialized == 1)
  {
    glPushMatrix();
      glTranslatef(m_position[0], m_position[1], m_position[2]);

      if (m_yaw != 0.0f) glRotatef(m_yaw, 0.0f, 1.0f, 0.0f);
      // ignoring pitch

      glCallList(m_dList1);        // render stand
      m_torus->vrender();
    glPopMatrix();
  }
  return;
}
