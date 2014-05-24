// Floor.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.20
	
    implementation of floor entity.
*/
#include "Floor.h"
//#include "vec3d.h"

// default constructor
Floor::Floor()
{
  m_length = m_width = 30;
  //m_initialized = 0;  
  m_texImage = NULL;
}

// size constructor
Floor::Floor(int x, int z)
{
  if (x > 0) m_length = x;
  else m_length = 20;
  if (z > 0) m_width = z;
  else m_width = 20;
  //m_initialized = 0; 
  m_texImage = NULL;
}

// copy constructor
Floor::Floor(const Floor &source)  : Entity(source)
{
  m_length = source.m_length;
  m_width = source.m_width;
  m_texImage = source.m_texImage;
  m_texID = source.m_texID;
  m_rect1 = source.m_rect1;
}

// destructor
Floor::~Floor()
{

}

// = operator overload
Floor & Floor::operator =(const Floor &source)
{
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent

    m_length = source.m_length;
    m_width = source.m_width;
    m_texImage = source.m_texImage;
    m_texID = source.m_texID;
    m_rect1 = source.m_rect1;
  }
  return(*this);
}

// ***************************************************************
// destroy
// unload entity data from memory
void Floor::vdestroy(void)
{
  m_texImage = NULL;
  m_initialized = 0;
  return;
}

// ****************************************************************
// init
// load data into memory including textures
int Floor::vinit(unsigned char *ptr_image, GLuint width, GLuint height) 
{
  float matAmbient[] = { 0.66f, 0.66f, 0.66f, 1.00f };    // material vars
  float matDiff[] =    { 0.95f, 0.95f, 0.95f, 1.00f };
  float matSpec[] =    { 1.00f, 1.00f, 1.00f, 1.00f };
  float matShiny = 90.0;
  GLfloat x = -m_length * 0.5f;               // for loop creation of quads
  GLfloat z = -m_width  * 0.5f;
  int i, j;                                // loop counter in quads creation

  //if (!ptr_image) return -1;               // can't be null, so quit

  // update the pointer to texture image
  
  m_texImage = ptr_image;

  // setup texture parameters
  glGenTextures(1, &m_texID);                               
  glBindTexture(GL_TEXTURE_2D, m_texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB,
         GL_UNSIGNED_BYTE, m_texImage);


    // generate list for rectangle
  m_rect1=glGenLists(1);
  glNewList(m_rect1, GL_COMPILE);
    // set materials
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, matShiny);
    glBindTexture(GL_TEXTURE_2D, m_texID);
	
    // make the floor multiple quads for shadows and reflections
    glNormal3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    for (i = 0; i < m_width; ++i)
    {
      for (j = 0; j < m_length; ++j)
      { 
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(x, -0.001f, z);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(x, -0.001f, z + 1.0f);
        x += 1.0f;
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(x, -0.001f, z + 1.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(x, -0.001f, z);
      }
      x = -m_length * 0.5f;                    // reset for next loop
      z += 1.0f;
    }
    glEnd();
  glEndList();

  m_initialized = 1;

  return m_initialized;
}


// ***********************************************************
// render
// Actually draw the floor
// *********************************************************
void Floor::vrender(void)
{
  if (m_initialized == 1 && m_texImage)
  {
    glPushMatrix();
      // don't check on position or orientation, not allowing floro to move.
      glCallList(m_rect1);
    glPopMatrix();
  }
  return;
}
