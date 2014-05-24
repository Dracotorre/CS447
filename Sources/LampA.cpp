// LampA.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.27
	
    implementation of LampA entity.
*/
#include "LampA.h"
#include "Vec3D.h"
#include "Mat3D.h"
#include "consts.h"            // for PI
#include "materials.h"         // for gold material

// default constructor
LampA::LampA()
{
  m_texImage1 = NULL;
  m_texID = -1;
  m_dList1= -1;
  m_stand = NULL;
}

// copy constructor
LampA::LampA(const LampA &source)  : Entity(source)
{
  m_texImage1 = source.m_texImage1;
  m_texID = -1;
  m_dList1 = -1;

  if (source.m_stand)
  {
    *m_stand = *source.m_stand;
  }
  else m_stand = NULL;
}

// ***************************
// destructor
LampA::~LampA()
{
  if (m_stand) delete m_stand;
}

// **************************************************
// = operator overload
LampA & LampA::operator =(const LampA &source)
{
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent

    m_texImage1 = source.m_texImage1;
    m_texID = -1;
    m_dList1 = -1;
    
    if (source.m_stand)
    {
      *m_stand = *source.m_stand;
    }
    else m_stand = NULL;
  }
  return(*this);
}


// ***************************************************************
// destroy
// unload entity data from memory
void LampA::vdestroy(void)
{
  m_texImage1 = NULL;
  m_initialized = 0;

  return;
}

// ****************************************************************
// init
// load data into memory including textures and build the
// display lists for rendering
//
int LampA::vinit(void)
{
  return vinit(NULL, 64, 64);
}

int LampA::vinit(unsigned char *ptr_image, GLuint width, GLuint height) 
{ 
  int slices = 8;               // slices around 
  int i = 0;
  float lampAmbient[] = { 0.6f, 0.66f, 0.80f, 1.0f };
  float lampDiff[]    = { 0.72f, 0.80f, 1.00f, 1.0f };
  float lampEmissBr[] = { 0.66f, 0.66f, 0.72f, 0.82f }; 
  float lampEmissT[]  = { 0.32f, 0.42f, 0.50f, 0.82f };
  float offEmission[] = { 0.0f, 0.00f, 0.00f, 1.0f }; 
  float lampBrColor[] = { 0.92f, 0.94f, 1.00f, 0.72f };
  float lampTipColor[]= { 0.64f, 0.70f, 0.78f, 0.8f };
  float lampYUmid= 1.700f;                              // for drawing lamp
  float lampYtop = 1.860f;
  float lampYLmid= 1.440f;
  float lampYmid = 1.560f;
  float lampYbot = 1.320f;
  float lampP1   = -0.180f;
  float lampP2   = 0.180f;
  float lampMid  = 0.120f;
  Mat3D rotation = rot_x(PI);    // to flip over before putting in cell
 
  // create cell decomposition of stone portion, in m_stand
  if (m_stand) delete m_stand;
  m_stand = new DSCells();
  
  // starting from bottom and building up
  DSCylinder cylA(0.200f, 0.200f, 0.040f, slices);
  m_stand->add(cylA);
  
  DSHyperboloid hypA(0.040f, 0.20f, 0.160f, slices);
  hypA.setRotation(rotation);
  hypA.setTranslation(Vec3D(0.0f, 0.200f, 0.0f));  // trans big since flipped
  m_stand->add(hypA);
  DSCylinder cylB(0.040f, 0.040f, 0.33333333f, slices);
  cylB.setTranslation(Vec3D(0.0f, 0.200f, 0.0f));
  m_stand->add(cylB);
  DSCylinder cylC = cylB;                         // repeat same cyl x3
  cylC.setTranslation(Vec3D(0.0f, 0.53333333f, 0.0f));
  m_stand->add(cylC);
  DSCylinder cylD = cylB;
  cylD.setTranslation(Vec3D(0.0f, 0.86666666f, 0.0f));
  m_stand->add(cylD);
  DSCylinder cylE(0.100f, 0.040f, 0.040f, slices);
  cylE.setTranslation(Vec3D(0.0f, 1.200f, 0.0f));   // base of lamp
  m_stand->add(cylE);
  
  DSCylinder cylF(0.160f, 0.100f, 0.120f, slices);
  cylF.setTranslation(Vec3D(0.0f, 1.2401f, 0.0f));   // base of lamp
  m_stand->add(cylF);
  
  DSCylinder cylG(0.080f, 0.160f, 0.040f, slices);
  cylG.setTranslation(Vec3D(0.0f, 1.3601f, 0.0f));   // base of lamp
  m_stand->add(cylG); 

  // setup textures and colors for stand
  if (ptr_image)
  {
    // update the pointer to texture image
    m_texImage1 = ptr_image;

    // setup texture parameters
    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, 
                      GL_UNSIGNED_BYTE, m_texImage1);
  }
  
  // setup top cap for stand
  rotation = rot_y(PI / slices);
  Vec3D pt(0.080f, 1.40f, 0.0f);     // intial pt on circle
  
  // start list for displaying later
  m_dList1 = glGenLists(1);
  glNewList(m_dList1, GL_COMPILE);
  glPushMatrix();                   
    // set materials for stand portion
    glMaterialfv(GL_FRONT, GL_AMBIENT, SHINY_GOLDAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, SHINY_GOLDDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SHINY_GOLDSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, SHINY_GOLDShiny);
    glBindTexture(GL_TEXTURE_2D, m_texID);

    m_stand->draw();

    
    glBegin(GL_TRIANGLES);      // top cap on stand top
    for (i = 0; i < slices; ++i)
    {
      glTexCoord2f(0.5f, 1.0f);
      glVertex3f(0.0f, 1.404f, 0.0f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(pt[0], pt[1], pt[2]);
      pt = pt * rotation;
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(pt[0], pt[1], pt[2]);
    }
    glEnd();

    // draw pyramid shaped lamp shade that is brighter in center of
    // each face.

    glMaterialfv(GL_FRONT, GL_AMBIENT, lampAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampDiff);
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissBr);  // it's bright

    glBegin(GL_TRIANGLE_FAN);              // front side top
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampBrColor);
    glVertex3f(0.0f, lampYUmid, lampMid);   
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampTipColor);
    glVertex3f(lampP1, lampYmid,  lampP2);
    glVertex3f(lampP2, lampYmid,  lampP2);
    glVertex3f(0.000f, lampYtop,  0.000f);
    glVertex3f(lampP1, lampYmid,  lampP2);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);              // front side bottom
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissBr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampBrColor);
    glVertex3f(0.0f, lampYLmid, lampMid); 
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampTipColor);
    glVertex3f(lampP2, lampYmid, lampP2);
    glVertex3f(lampP1, lampYmid, lampP2);
    glVertex3f(0.000f, lampYbot, 0.000f);
    glVertex3f(lampP2, lampYmid,  lampP2);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);              // right side bottom
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissBr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampBrColor);
    glVertex3f(lampMid, lampYLmid, 0.0f);
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampTipColor);
    glVertex3f(lampP2, lampYmid, lampP2);
    glVertex3f(0.000f, lampYbot, 0.000f);
    glVertex3f(lampP2, lampYmid, lampP1);
    glVertex3f(lampP2, lampYmid, lampP2);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);              // right side top
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissBr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampBrColor);
    glVertex3f(lampMid, lampYUmid, 0.0f);
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampTipColor);
    glVertex3f(lampP2, lampYmid, lampP2);
    glVertex3f(lampP2, lampYmid, lampP1);
    glVertex3f(0.000f, lampYtop,  0.000f);
    glVertex3f(lampP2, lampYmid, lampP2);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);              // back side bottom
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissBr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampBrColor);
    glVertex3f(0.0f, lampYLmid, -lampMid);
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampTipColor);
    glVertex3f(lampP2, lampYmid, lampP1);
    glVertex3f(0.000f, lampYbot, 0.000f);
    glVertex3f(lampP1, lampYmid, lampP1);
    glVertex3f(lampP2, lampYmid, lampP1);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);              // back side top
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissBr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampBrColor);
    glVertex3f(0.0f, lampYUmid, -lampMid);
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampTipColor);
    glVertex3f(lampP2, lampYmid, lampP1);
    glVertex3f(lampP1, lampYmid, lampP1);
    glVertex3f(0.000f, lampYtop,  0.000f);
    glVertex3f(lampP2, lampYmid, lampP1);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);              // left side top
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissBr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampBrColor);
    glVertex3f(-lampMid, lampYUmid, 0.0f);
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampTipColor);
    glVertex3f(lampP1, lampYmid, lampP1);
    glVertex3f(lampP1, lampYmid, lampP2);
    glVertex3f(0.000f, lampYtop,  0.000f);
    glVertex3f(lampP1, lampYmid, lampP1);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);              // left side bottom
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissBr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampBrColor);
    glVertex3f(-lampMid, lampYLmid, 0.0f);
    glMaterialfv(GL_FRONT, GL_EMISSION, lampEmissT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lampTipColor);
    glVertex3f(lampP1, lampYmid, lampP1);
    glVertex3f(0.000f, lampYbot, 0.000f);
    glVertex3f(lampP1, lampYmid, lampP2);
    glVertex3f(lampP1, lampYmid, lampP1);
    glEnd();
    glMaterialfv(GL_FRONT, GL_EMISSION, offEmission);  // turn back off
    
  glPopMatrix(); 

  glEndList();
  
  // set BB for entity: the lamp on top of stand is the widest and tallest
  setBBox(Vec3D(lampP1, lampYtop, lampP1), Vec3D(lampP2, 0.0f, lampP2));

  m_initialized = 1;

  return m_initialized;
}


// ***********************************************************
// render
void LampA::vrender(void)
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
