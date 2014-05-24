// Pawn.cpp
/*
    David Shrock
    CS447 - PSU
    2004.08.05
	
    implementation of Pawn entity.
*/
#include "Pawn.h"
#include "materials.h"         // for gold material

// default constructor
Pawn::Pawn()
{
  m_texImage1 = NULL;
  m_texID = -1;
  m_dList1= -1;
  m_pawn = NULL;
}

// copy constructor
Pawn::Pawn(const Pawn &source)  : Entity(source)
{
  m_texImage1 = source.m_texImage1;
  m_texID = -1;
  m_dList1 = -1;

  if (source.m_pawn)
  {
    *m_pawn = *source.m_pawn;
  }
  else m_pawn = NULL;
}

// ***************************
// destructor
Pawn::~Pawn()
{
  if (m_pawn) delete m_pawn;
}

// **************************************************
// = operator overload
Pawn & Pawn::operator =(const Pawn &source)
{
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent

    m_texImage1 = source.m_texImage1;
    m_texID = -1;
    m_dList1 = -1;
    
    if (source.m_pawn)
    {
      *m_pawn = *source.m_pawn;
    }
    else m_pawn = NULL;
  }
  return(*this);
}


// ***************************************************************
// destroy
// unload entity data from memory
void Pawn::vdestroy(void)
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
int Pawn::vinit(void) 
{ 
  int i = 0;
  Vertex verts[26];
  verts[0].x = 0.0f;
  verts[0].y = 0.0f;
  verts[1].x = 0.35f;
  verts[1].y = 0.0f;
  verts[2].x = 0.35f;
  verts[2].y = 0.10f;
  verts[3].x = 0.30f;
  verts[3].y = 0.10f;
  verts[4].x = 0.33f;
  verts[4].y = 0.113f;
  verts[5].x = 0.35f;
  verts[5].y = 0.15f;
  verts[6].x = 0.33f;
  verts[6].y = 0.188f;
  verts[7].x = 0.30f;
  verts[7].y = 0.20f;
  verts[8].x = 0.205f;
  verts[8].y = 0.26f;
  verts[9].x = 0.185f;
  verts[9].y = 0.30f;
  verts[10].x = 0.20f;
  verts[10].y = 0.35f;
  verts[11].x = 0.16f;
  verts[11].y = 0.375f;
  verts[12].x = 0.125f;
  verts[12].y = 0.45f;
  verts[13].x = 0.10f;
  verts[13].y = 0.55f;
  verts[14].x = 0.095f;
  verts[14].y = 0.675f;
  verts[15].x = 0.175f;
  verts[15].y = 0.675f;
  verts[16].x = 0.203f;
  verts[16].y = 0.688f;
  verts[17].x = 0.203f;
  verts[17].y = 0.713f;
  verts[18].x = 0.175f;
  verts[18].y = 0.730f;
  verts[19].x = 0.125f;
  verts[19].y = 0.750f;
  verts[20].x = 0.175f;
  verts[20].y = 0.800f;
  verts[21].x = 0.200f;
  verts[21].y = 0.850f;
  verts[22].x = 0.200f;
  verts[22].y = 0.975f;
  verts[23].x = 0.175f;
  verts[23].y = 1.050f;
  verts[24].x = 0.100f;
  verts[24].y = 1.10f;
  verts[25].x = 0.0f;
  verts[25].y = 1.12f;
  for (i = 0; i < 26; ++i)
  {
    verts[i].z = 0.0f;
  }
  Poly poly(verts[0], verts[1], verts[2], 25);

  for (i = 3; i < 26; ++i)
  {
    poly.addPoint(verts[i]);
  }
 
  // create cell decomposition of stone portion, in m_pawn
  if (m_pawn) delete m_pawn;
  m_pawn = new DSLathe(poly);

  // setup textures and colors for stand
  /*
  if (ptr_image)
  {
    // update the pointer to texture image
   // m_texImage1 = ptr_image;
  
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
  }  */

  // start list for displaying later
  m_dList1 = glGenLists(1);
  glNewList(m_dList1, GL_COMPILE);
  glPushMatrix();                   
    // set materials for stand portion
    glMaterialfv(GL_FRONT, GL_AMBIENT, REDAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, REDDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, REDSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, REDShiny);
    //glBindTexture(GL_TEXTURE_2D, m_texID);

    m_pawn->vdraw();
    
  glPopMatrix(); 

  glEndList();
  
  // set BB for entity: the lamp on top of stand is the widest and tallest
  setBBox(Vec3D(0.35f, 1.8f, 0.35f), Vec3D(-0.35f, 0.0f, -0.35f));

  m_initialized = 1;

  return m_initialized;
}


// ***********************************************************
// render
void Pawn::vrender(void)
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
