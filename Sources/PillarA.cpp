// PillarA.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.26
	
    implementation of PillarA entity.
*/
#include "PillarA.h"
#include "Vec3D.h"
#include "Mat3D.h"
#include "consts.h"            // for PI
#include "materials.h"         // for gold material

// default constructor
PillarA::PillarA()
{
  m_texImage1 = NULL;
  m_stoneID = -1;
  m_dList1= -1;
  m_torus = NULL;
  m_pil = NULL;
}

// copy constructor
PillarA::PillarA(const PillarA &source)  : Entity(source)
{
  m_texImage1 = source.m_texImage1;
  m_stoneID = -1;
  m_dList1 = -1;
  if (source.m_torus)
  {
    *m_torus = *source.m_torus;
  }
  else m_torus = NULL;
  if (source.m_pil)
  {
    *m_pil = *source.m_pil;
  }
  else m_pil = NULL;
}

// ***************************
// destructor
PillarA::~PillarA()
{
  if (m_torus) delete m_torus;
  if (m_pil) delete m_pil;
}

// **************************************************
// = operator overload
PillarA & PillarA::operator =(const PillarA &source)
{
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent

    m_texImage1 = source.m_texImage1;
    m_stoneID = -1;
    m_dList1 = -1;
    if (source.m_torus)
    {
      *m_torus = *source.m_torus;
    }
    else m_torus = NULL;
    if (source.m_pil)
    {
      *m_pil = *source.m_pil;
    }
    else m_pil = NULL;
  }
  return(*this);
}

// ***************************************************************
// destroy
// unload entity data from memory
void PillarA::vdestroy(void)
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
int PillarA::vinit(void)
{
  return vinit(NULL, 64, 64);
}

int PillarA::vinit(unsigned char *ptr_image, GLuint width, GLuint height) 
{ 
  int slices = 24;           // slices around for stone pillar
  Vertex bbmax, bbmin;
  Mat3D rotation = rot_x(PI);    // to flip over before putting in cell
 
  // create cell decomposition of stone portion, in m_pil
  if (m_pil) delete m_pil;
  m_pil = new DSCells();
  
  // starting from bottom and building up
  DSCylinder cylA(0.60f, 0.60f, 0.20f, slices);
  m_pil->add(cylA);
  DSHyperboloid hypA(0.40f, 0.60f, 0.30f, slices);
  hypA.setRotation(rotation);
  hypA.setTranslation(Vec3D(0.0f, 0.50f, 0.0f)); 
  m_pil->add(hypA);
  DSCylinder cylB(0.40f, 0.40f, 0.50f, slices);
  cylB.setTranslation(Vec3D(0.0f, 0.50f, 0.0f));
  m_pil->add(cylB);
  DSCylinder cylC(0.3f, 0.4f, 0.1f, slices);
  cylC.setTranslation(Vec3D(0.0f, 1.0f, 0.0f));
  m_pil->add(cylC);
  DSCylinder cylD(0.3f, 0.3f, 1.5f, slices);      // tall mid piece in 2
  cylD.setTranslation(Vec3D(0.0f, 1.1f, 0.0f));
  m_pil->add(cylD);
  DSCylinder cylD2(0.3f, 0.3f, 1.4f, slices);     // tall mid part 2
  cylD2.setTranslation(Vec3D(0.0f, 2.6f, 0.0f));
  m_pil->add(cylD2);
  
  DSCylinder cylE(0.4f, 0.3f, 0.1f, slices);
  cylE.setTranslation(Vec3D(0.0f, 4.0f, 0.0f));
  m_pil->add(cylE);
  DSCylinder cylF(0.4f, 0.4f, 0.4f, slices);
  cylF.setTranslation(Vec3D(0.0f, 4.1f, 0.0f));
  m_pil->add(cylF);
  DSHyperboloid hypB(0.4f, 0.6f, 0.3f, slices);
  hypB.setTranslation(Vec3D(0.0f, 4.5f, 0.0f));
  m_pil->add(hypB);
  DSCylinder cylG(0.7f, 0.6f, 0.1f, slices);
  cylG.setTranslation(Vec3D(0.0f, 4.8f, 0.0f));
  m_pil->add(cylG);
  DSCylinder cylH(0.7f, 0.7f, 0.1f, slices);
  cylH.setTranslation(Vec3D(0.0f, 4.9f, 0.0f));
  m_pil->add(cylH);

  // build textures if requested
  if (ptr_image)
  {
    // update the pointer to texture image
  
    m_texImage1 = ptr_image;

    // setup texture parameters
    glGenTextures(1, &m_stoneID);
    glBindTexture(GL_TEXTURE_2D, m_stoneID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      //              GL_NEAREST_MIPMAP_LINEAR);
	                 GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, m_texImage1);
  }
  
  // generate list for pilar
  m_dList1=glGenLists(1);
  glNewList(m_dList1, GL_COMPILE);

  glPushMatrix();                     // stone portion
    // set materials
    glMaterialfv(GL_FRONT, GL_AMBIENT, STONE_WALLAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, STONE_WALLDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, STONE_WALLSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, STONE_WALLShiny);
    glBindTexture(GL_TEXTURE_2D, m_stoneID);

    m_pil->draw();

    // place top on m_pill cell structure, building up (see sketch)
    // ignoring polygons facing up since tall, can't see top
    glBegin(GL_QUADS);
    // front side
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.5f, 0.0f);         // facing out
    glVertex3f(-0.5f, 5.0f, 0.5f);
    glTexCoord2f(3.5f, 0.0f);
    glVertex3f(0.5f, 5.0f, 0.5f);
    glTexCoord2f(3.5f, 2.0f);
    glVertex3f(0.5f, 5.4f, 0.5f);
    glTexCoord2f(0.5f, 2.0f);
    glVertex3f(-0.5f, 5.4f, 0.5f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);           // facing down
    glVertex3f(-0.7f, 5.4f, 0.5f);
    glTexCoord2f(4.0f, 0.0f);
    glVertex3f(0.7f, 5.4f, 0.5f);
    glTexCoord2f(4.0f, 1.0f);
    glVertex3f(0.7f, 5.4f, 0.7f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.7f, 5.4f, 0.7f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);           // facing out
    glVertex3f(-0.7f, 5.4f, 0.7f);
    glTexCoord2f(4.0f, 0.0f);
    glVertex3f(0.7f, 5.4f, 0.7f);
    glTexCoord2f(4.0f, 1.0f);
    glVertex3f(0.7f, 5.6f, 0.7f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.7f, 5.6f, 0.7f);
    // right side
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.5f, 0.0f);         // facing out
    glVertex3f(0.5f, 5.0f, 0.5f);
    glTexCoord2f(3.5f, 0.0f);
    glVertex3f(0.5f, 5.0f, -0.5f);
    glTexCoord2f(3.5f, 2.0f);
    glVertex3f(0.5f, 5.4f, -0.5f);
    glTexCoord2f(0.5f, 2.0f);
    glVertex3f(0.5f, 5.4f, 0.5f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);           // facing down
    glVertex3f(0.5f, 5.4f, 0.7f);
    glTexCoord2f(4.0f, 0.0f);
    glVertex3f(0.5f, 5.4f, -0.7f);
    glTexCoord2f(4.0f, 1.0f);
    glVertex3f(0.7f, 5.4f, -0.7f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.7f, 5.4f, 0.7f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);           // facing out
    glVertex3f(0.7f, 5.4f, 0.7f);
    glTexCoord2f(4.0f, 0.0f);
    glVertex3f(0.7f, 5.4f, -0.7f);
    glTexCoord2f(4.0f, 1.0f);
    glVertex3f(0.7f, 5.6f, -0.7f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.7f, 5.6f, 0.7f);
    // back side
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.5f, 0.0f);         // facing out
    glVertex3f(0.5f, 5.0f, -0.5f);
    glTexCoord2f(3.5f, 0.0f);
    glVertex3f(-0.5f, 5.0f, -0.5f);
    glTexCoord2f(3.5f, 2.0f);
    glVertex3f(-0.5f, 5.4f, -0.5f);
    glTexCoord2f(0.5f, 2.0f);
    glVertex3f(0.5f, 5.4f, -0.5f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);           // facing down
    glVertex3f(0.7f, 5.4f, -0.5f);
    glTexCoord2f(4.0f, 0.0f);
    glVertex3f(-0.7f, 5.4f, -0.5f);
    glTexCoord2f(4.0f, 1.0f);
    glVertex3f(-0.7f, 5.4f, -0.7f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.7f, 5.4f, -0.7f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);           // facing out
    glVertex3f(0.7f, 5.4f, -0.7f);
    glTexCoord2f(4.0f, 0.0f);
    glVertex3f(-0.7f, 5.4f, -0.7f);
    glTexCoord2f(4.0f, 1.0f);
    glVertex3f(-0.7f, 5.6f, -0.7f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.7f, 5.6f, -0.7f);
    // left side
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(-0.5f, 0.0f);         // facing out
    glVertex3f(-0.5f, 5.0f, -0.5f);
    glTexCoord2f(3.5f, 0.0f);
    glVertex3f(-0.5f, 5.0f, 0.5f);
    glTexCoord2f(3.5f, 2.0f);
    glVertex3f(-0.5f, 5.4f, 0.5f);
    glTexCoord2f(0.5f, 2.0f);
    glVertex3f(-0.5f, 5.4f, -0.5f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);           // facing down
    glVertex3f(-0.5f, 5.4f, -0.7f);
    glTexCoord2f(4.0f, 0.0f);
    glVertex3f(-0.5f, 5.4f, 0.7f);
    glTexCoord2f(4.0f, 1.0f);
    glVertex3f(-0.7f, 5.4f, 0.7f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.7f, 5.4f, -0.7f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);           // facing out
    glVertex3f(-0.7f, 5.4f, -0.7f);
    glTexCoord2f(4.0f, 0.0f);
    glVertex3f(-0.7f, 5.4f, 0.7f);
    glTexCoord2f(4.0f, 1.0f);
    glVertex3f(-0.7f, 5.6f, 0.7f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.7f, 5.6f, -0.7f);
    glEnd();

  glPopMatrix();

  glEndList();
  
  // update bounding box for this Entity
  m_pil->getBB(bbmax, bbmin);
  setBBox(Vec3D(bbmax.x, 5.6f, bbmax.z), Vec3D(bbmin.x, 0.0f, bbmin.z));

  m_initialized = 1;

  return m_initialized;
}


// ***********************************************************
// render
// *********************************************************
void PillarA::vrender(void)
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

