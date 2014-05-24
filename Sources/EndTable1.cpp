// EndTable1.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.29

    implementation of EndTable1 Entity.
*/
#include "EndTable1.h"
#include "materials.h"         // for gold material

// default constructor
EndTable1::EndTable1()
{
  m_texImage1 = NULL;
  m_texID = -1;
  m_dList1= -1;
  m_top = NULL;
  m_leg = NULL;
  m_toy = NULL;
}

// copy constructor
EndTable1::EndTable1(const EndTable1 &source)  : Entity(source)
{
  m_texImage1 = source.m_texImage1;
  m_texID = -1;
  m_dList1 = -1;
  if (source.m_top) 
  {
    m_top = new DSLathe();
    *m_top = *source.m_top;
  }
  else m_top = NULL;
  if (source.m_leg)
  {
    m_leg = new DSLathe();
    *m_leg = *source.m_leg;
  }
  else m_leg = NULL;
  if (source.m_toy)
  {
    m_toy = new DSExtrude();
    *m_toy = *source.m_toy;
  }
  else m_toy = NULL;
}

// ***************************
// destructor
EndTable1::~EndTable1()
{
  if (m_top) delete m_top;
  if (m_leg) delete m_leg;
  if (m_toy) delete m_toy;
}

// **************************************************
// = operator overload
EndTable1 & EndTable1::operator =(const EndTable1 &source)
{
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent
    m_texImage1 = source.m_texImage1;
    m_texID = -1;
    m_dList1 = -1;
    if (source.m_top) 
    {
      m_top = new DSLathe();
      *m_top = *source.m_top;
    }
    else m_top = NULL;
    if (source.m_leg)
    {
      m_leg = new DSLathe();
      *m_leg = *source.m_leg;
    }
    else m_leg = NULL;
    if (source.m_toy)
    {
      m_toy = new DSExtrude();
      *m_toy = *source.m_toy;
    }
    else m_toy = NULL;
  }
  return(*this);
}

// ***************************************************************
// destroy
// unload entity data from memory
void EndTable1::vdestroy(void)
{
  m_texImage1 = NULL;
  m_initialized = 0;

  return;
}

// ****************************************************************
// init
//
int EndTable1::vinit(void)
{
  float toyDiff[] = { 1.0f, 0.2f, 0.24f, 1.0f };
  float toyAmbient[] = { 0.2f, 0.1f, 0.1f, 1.0f };
  float tableDiff[] = { 0.75f, 0.45f, 0.10f, 1.0f };
  float tableAmbient[] = { 0.1f, 0.05f, 0.05f, 1.0f };
  float tableSpec[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  float tableShiny = 80.0f;
  buildTop();                // create the part from Poly
  buildLeg();
  buildToy();

  // generate list to capture drawing
  m_dList1=glGenLists(1);
  glNewList(m_dList1, GL_COMPILE);

  glPushMatrix();                  
    // set materials
    glMaterialfv(GL_FRONT, GL_AMBIENT, tableAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, tableDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, tableSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, tableShiny);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    
    m_top->vdraw();            
    m_leg->vdraw();

    glMaterialfv(GL_FRONT, GL_AMBIENT, toyAmbient);
    glMaterialfv(GL_FRONT, GL_AMBIENT, toyDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SHINY_GOLDSpec);

    m_toy->vdraw();

  glPopMatrix();

  glEndList();
  
  // update bounding box for this Entity
  // setting too tall for camera (should adjust camera)
  setBBox(Vec3D(0.50f, 1.757f, 0.50f), Vec3D(-0.50f, 0.0f, -0.50f));

  m_initialized = 1;

  return m_initialized;
}


// ***********************************************************
// render
// *********************************************************
void EndTable1::vrender(void)
{
  if (m_initialized == 1)
  {
    glPushMatrix();
      glTranslatef(m_position[0], m_position[1], m_position[2]);
       if (m_yaw != 0.0f) 
      {
        glRotatef(m_yaw, 0.0f, 1.0f, 0.0f);
      }
      // skipping pitch
      glCallList(m_dList1);
    glPopMatrix();
  }
  return;
}

// **********************************************************************
// private functions
// ********************

// *******************************************************************
// buildLeg
/* picking points for curve from sketch on graph paper. It would also
   be interesting to make a bezier curve to define the points.
*/
void EndTable1::buildLeg(void)
{
  int i = 0;
  Vertex pts[21];                 // points for Poly to lathe
  
  // DSLathe requires that Poly is created in counter-clockwise order on
  // +x side of axis on z=0 plane. Starting at bottom. See sketch.
  pts[0].x  = 0.000f;
  pts[0].y  = 0.000f;
  pts[1].x  = 0.300f;
  pts[1].y  = 0.000f;
  pts[2].x  = 0.250f;
  pts[2].y  = 0.058f;
  pts[3].x  = 0.175f;
  pts[3].y  = 0.075f;
  pts[4].x  = 0.112f;
  pts[4].y  = 0.150f;
  pts[5].x  = 0.080f;
  pts[5].y  = 0.275f;
  pts[6].x  = 0.100f;
  pts[6].y  = 0.350f;
  pts[7].x  = 0.125f;
  pts[7].y  = 0.375f;
  pts[8].x  = 0.100f;
  pts[8].y  = 0.400f;
  pts[9].x  = 0.080f;
  pts[9].y  = 0.450f;
  pts[10].x = 0.075f;
  pts[10].y = 0.525f;
  pts[11].x = 0.112f;
  pts[11].y = 0.555f;
  pts[12].x = 0.1125f;
  pts[12].y = 0.620f;
  pts[13].x = 0.0725f;
  pts[13].y = 0.655f;
  pts[14].x = 0.100f;
  pts[14].y = 0.725f;
  pts[15].x = 0.1325f;
  pts[15].y = 0.800f;
  pts[16].x = 0.125f;
  pts[16].y = 0.875f;
  pts[17].x = 0.100f;
  pts[17].y = 0.925f;
  pts[18].x = 0.075f;
  pts[18].y = 1.000f;
  pts[19].x = 0.075f;
  pts[19].y = 1.075f;
  pts[20].x = 0.000f;
  pts[20].y = 1.075f;
  for (i = 0; i < 21; ++i)
  {
    pts[i].z = 0.00f;
  }

  // set up the polygon for lathing
  Poly leg(pts[0], pts[1], pts[2], 21);
  for (i = 3; i < 21; ++i)
  {
    leg.addPoint(pts[i]);
  }
  if (!m_leg)
  {
    m_leg = new DSLathe(leg, 16);     // try 16 slices around
  }
  return;
}

// *****************************************************************
// buildTop
void EndTable1::buildTop(void)
{
  Vertex pts[4];                // points for Poly to extrude
  
  // starting on top left side, going cc-wise
  pts[0].x = 0.0f;
  pts[0].y = 1.075f;
  pts[1].x = 0.50f;
  pts[1].y = 1.075f;
  pts[2].x = 0.50f;
  pts[2].y = 1.175f;
  pts[3].x = 0.0f;
  pts[3].y = 1.175f;
  for (int i = 0; i < 4; ++i)
  {
    pts[i].z = 0.0f;
  }

  // do Poly
  Poly top(pts[0], pts[1], pts[2], 4);
  top.addPoint(pts[3]);

  if (!m_top)
  {
    m_top = new DSLathe(top, 24);
  }

  return;
}

// ******************************************
void EndTable1::buildToy(void)
{
  Vertex pts[5];      // making a little hexagon thing
  pts[0].x = 0.20f;
  pts[0].y = 1.175f;
  pts[1].x = 0.312f;
  pts[1].y = 1.175f;
  pts[2].x = 0.336f;
  pts[2].y = 1.288f;
  pts[3].x = 0.24f;
  pts[3].y = 1.336f;
  pts[4].x = 0.144f;
  pts[4].y = 1.264f;
  for (int i = 0; i < 5; ++i)
  {
    pts[i].z = 0.0f;
  }
  Poly toy(pts[0], pts[1], pts[2], 5);
  toy.addPoint(pts[3]);
  toy.addPoint(pts[4]);

  if (!m_toy) m_toy = new DSExtrude(toy, 0.1f);
  

  return;
}


