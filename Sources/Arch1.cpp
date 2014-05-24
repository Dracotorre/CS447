// Arch1.h
/*
    David Shrock
    CS447 - PSU
    2004.07.28
	
    implementation of Arch1
*/
#include "Arch1.h"
#include "Vec3D.h"
#include "Mat3D.h"
#include "consts.h"            // for PI
#include "materials.h"         // for stone material

// default constructor
Arch1::Arch1()
{
  m_texImage1 = NULL;
  m_stoneID = -1;
  m_dList1= -1;
}

// copy constructor
Arch1::Arch1(const Arch1 &source)  : Entity(source)
{
  m_texImage1 = source.m_texImage1;
  m_stoneID = source.m_stoneID;
  m_dList1 = source.m_dList1;
}

// ***************************
// destructor
Arch1::~Arch1()
{

}

// **************************************************
// = operator overload
Arch1 & Arch1::operator =(const Arch1 &source)
{
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent

    m_texImage1 = source.m_texImage1;
    m_stoneID = source.m_stoneID;
    m_dList1 = source.m_dList1;
  }
  return(*this);
}

// ***************************************************************
// destroy
// unload entity data from memory
void Arch1::vdestroy(void)
{
  m_texImage1 = NULL;
  m_initialized = 0;

  return;
}

// ****************************************************************
// init
// load data and build the
// display lists for rendering
//
int Arch1::vinit(unsigned char *ptr_image, GLuint width, GLuint height) 
{ 
 
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
  
  // generate list
  m_dList1=glGenLists(1);
  glNewList(m_dList1, GL_COMPILE);

  // set materials
  glMaterialfv(GL_FRONT, GL_AMBIENT, STONE_WALLAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, STONE_WALLDiff);
  glMaterialfv(GL_FRONT, GL_SPECULAR, STONE_WALLSpec);
  glMaterialf(GL_FRONT, GL_SHININESS, STONE_WALLShiny);
  glBindTexture(GL_TEXTURE_2D, m_stoneID);

  // front side
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-3.8f, 1.0f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-3.8f, 0.0f, 0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-3.2f, 1.0f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-3.2f, 0.0f, 0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-2.6f, 1.0f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-2.6f, 0.25f, 0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-2.0f, 1.0f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-2.0f, 0.40f, 0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-1.50f, 1.0f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-1.50f, 0.50f, 0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.70f, 1.0f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.70f, 0.60f, 0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.20f, 1.0f, 0.3f);            // mid
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.20f, 0.660f, 0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.20f, 1.0f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.20f, 0.660f, 0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.70f, 1.0f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.70f, 0.60f, 0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(1.50f, 1.0f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(1.50f, 0.50f, 0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(2.0f, 1.0f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(2.0f, 0.40f, 0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(2.6f, 1.0f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(2.6f, 0.25f, 0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(3.2f, 1.0f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(3.2f, 0.0f, 0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(3.8f, 1.0f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(3.8f, 0.0f, 0.3f);
  glEnd();


  // bottom side
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3f(0.0f, -1.0f, 0.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-3.2f, 0.0f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-3.2f, 0.0f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-2.6f, 0.25f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-2.6f, 0.25f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-2.0f, 0.40f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-2.0f, 0.40f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-1.50f, 0.50f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-1.50f, 0.50f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.70f, 0.60f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.70f, 0.60f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.20f, 0.660f, 0.3f);            // mid
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.20f, 0.660f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.20f, 0.660f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.20f, 0.660f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.70f, 0.60f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.70f, 0.60f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(1.50f, 0.50f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(1.50f, 0.50f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(2.0f, 0.40f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(2.0f, 0.40f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(2.6f, 0.25f, 0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(2.6f, 0.25f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(3.2f, 0.0f, 0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(3.2f, 0.0f, -0.3f);
  glEnd();

  // back side - mitered back, so start inward
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3f(0.0f, 0.0f, -1.0f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(3.2f, 1.0f, -0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(3.2f, 0.0f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(2.6f, 1.0f, -0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(2.6f, 0.25f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(2.0f, 1.0f, -0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(2.0f, 0.40f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(1.50f, 1.0f, -0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(1.50f, 0.50f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(0.70f, 1.0f, -0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.70f, 0.60f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(0.20f, 1.0f, -0.3f);            // mid
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(0.20f, 0.660f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.20f, 1.0f, -0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.20f, 0.660f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.70f, 1.0f, -0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.70f, 0.60f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-1.50f, 1.0f, -0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-1.50f, 0.50f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-2.0f, 1.0f, -0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-2.0f, 0.40f, -0.3f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-2.6f, 1.0f, -0.3f);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-2.6f, 0.25f, -0.3f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-3.2f, 1.0f, -0.3f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-3.2f, 0.0f, -0.3f);
  glEnd();

  glEndList();
  
  // update bounding box for this Entity
  setBBox(Vec3D(3.8f, 1.0f, 0.3f), Vec3D(-3.8f, 0.0f, -0.3f));

  m_initialized = 1;

  return m_initialized;
}


// ***********************************************************
// render
// 
void Arch1::vrender(void)
{
  if (m_initialized == 1)
  {
    glPushMatrix();
      if (m_yaw != 0.0f) 
      {
        glRotatef(m_yaw, 0.0f, 1.0f, 0.0f);
      }
      // skipping pitch
      glTranslatef(m_position[0], m_position[1], m_position[2]);
      glCallList(m_dList1);
    glPopMatrix();
  }
  return;
}

