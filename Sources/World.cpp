// World.cpp
/*
  David Shrock
  CS447 - PSU
  2004.08.01

  implementation of World.
*/
#include "World.h"

// **************************
// constructor
World::World()
{
  m_targas = NULL;
  m_cam = new Camera;
  m_lamp = new LampA;
  m_dome = new Dome;
  m_floor = new Floor();
  m_endTable = new EndTable1();
  m_torus = new TorStand();
  m_pawn = new Pawn();
  m_dList = -1;
  m_lightPosition[0] = 0.0f;
  m_lightPosition[1] = 3.0f;
  m_lightPosition[2] = 0.0f;
  m_lightPosition[3] = 1.0f;
  setShadowMatrix();      // calculate shadow matrix to get m_shadowMatrix
  //m_viewList = new EntityList;
}

// ********************************
// copy constructor
World::World(const World &source) : Entity(source)
{
  int i = 0;
  for (i = 0; i < 4; ++i)
  {
    m_idList[i] = source.m_idList[i];
    m_lightPosition[i] = source.m_lightPosition[i];
  }
  for (i = 0; i < 16; ++i)
  {
    m_shadowMatrix[i] = source.m_shadowMatrix[i];
  }
  if (source.m_targas)
  {
    m_targas = new TgaList(source.m_targas->getSize());
    *m_targas = *source.m_targas;
  }
  else m_targas = NULL;
  if (source.m_cam)
  {
    m_cam = new Camera();
    *m_cam = *source.m_cam;
  }
  else m_cam = NULL;
  if (source.m_dome)
  {
    m_dome = new Dome();
    *m_dome = *source.m_dome;
  }
  else m_dome = NULL;
  if (source.m_floor)
  {
    m_floor = new Floor(20, 20);  // size to room
    *m_floor = *source.m_floor;
  }
  else m_floor = NULL;
  if (source.m_lamp)
  {
    m_lamp = new LampA();
    *m_lamp = *source.m_lamp;
  }
  else m_lamp = NULL;
  if (source.m_endTable)
  {
    m_endTable = new EndTable1();
    *m_endTable = *source.m_endTable;
  }
  else m_endTable = NULL;
  if (source.m_torus)
  {
    m_torus = new TorStand();
    *m_torus = *source.m_torus;
  }
  else m_torus = NULL;
  m_pawn = new Pawn();
  /*if (source.m_viewList)
  {
    m_viewList = new EntityList();
    *m_viewList = *source.m_viewList;
  }
  else m_viewList = NULL;
  */

}

// ***********************************
World::~World()
{
  //if (m_viewList) delete m_viewList;
  if (m_targas) delete m_targas;
  if (m_cam) delete m_cam;
  if (m_dome) delete m_dome;
  if (m_floor) delete m_floor;
  if (m_lamp) delete m_lamp; 
  if (m_endTable) delete m_endTable;
  if (m_torus) delete m_torus;
  if (m_pawn) delete m_pawn;
}

// **********************************************
// assignment op, in case we need to copy the world
World & World::operator =(const World &source)
{
  int i = 0;
  if (this != &source)
  {
    static_cast<Entity &>(*this) = source;        // call parent
    for (i = 0; i < 4; ++i)
    {
      m_idList[i] = source.m_idList[i];
      m_lightPosition[i] = source.m_lightPosition[i];
    }
    for (i = 0; i < 16; ++i)
    {
      m_shadowMatrix[i] = source.m_shadowMatrix[i];
    }
    if (source.m_targas)
    {
      m_targas = new TgaList(source.m_targas->getSize());
      *m_targas = *source.m_targas;
    }
    else m_targas = NULL;
    if (source.m_cam)
    {
      m_cam = new Camera();
      *m_cam = *source.m_cam;
    }
    else m_cam = NULL;
    if (source.m_dome)
    {
      m_dome = new Dome();
      *m_dome = *source.m_dome;
    }
    else m_dome = NULL;
    if (source.m_floor)
    {
      m_floor = new Floor();
      *m_floor = *source.m_floor;
    }
    else m_floor = NULL;
    if (source.m_lamp)
    {
      m_lamp = new LampA();
      *m_lamp = *source.m_lamp;
    }
    else m_lamp = NULL;
    if (source.m_endTable)
    {
      m_endTable = new EndTable1();
      *m_endTable = *source.m_endTable;
    }
    else m_endTable = NULL;
    if (source.m_torus)
    {
      m_torus = new TorStand();
      *m_torus = *source.m_torus;
    }
    else m_torus = NULL;
    m_pawn = new Pawn();
    /*
    if (source.m_viewList)
    {
      m_viewList = new EntityList();
      *m_viewList = *source.m_viewList;
    }
    else m_viewList = NULL;
    */
  }
  return (*this);
}

// **********************************************************************
/* set camera deltas
   returns -1 for not set: collision or 1 for set ok
   Must interpret deltas for this world and send resulting deltas to Cam
   incomming deltas referenced to Cam coords facing -z at origin
   deltaPos should only be in directions aligned with axis with -z meaning 
   foreward.
*/
int World::moveCam(const Vec3D &deltaPos, float deltaPitch, float deltaYaw)
{
  int result = 1;
  int col = -1;              // for collision test
  Camera camBack = *m_cam;    // back it up

  // set cam
  if (result == 1) m_cam->setCamDelta(deltaPos, deltaPitch, deltaYaw);

  // check to see if collision
  col = camCollisionTest(m_cam->getPosition(), m_cam->getBoundRadius());

  if (col == 1) *m_cam = camBack;

  // decide if anything needs to be removed or added to the view list

  return result;
}

// **********************************************************************
// init
int World::vinit(void)
{
  // set lighting variables
  float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };         // ambient element of light
  float diffuseLight[] = { 0.85f, 0.85f, 0.85f, 1.0f };      // diffuse element of light
  float fogColor[]     = { 0.50f, 0.50f, 0.50f, 1.0f };
  int rockTex = -1;
	int stoneTex = -1;
  int marbTex = -1;
  int wallTex = -1;
 
  // load the textures for this world
  m_targas = new TgaList(6);
	marbTex = m_targas->add("textures/marbrose.tga");
	wallTex = m_targas->add("textures/rockwall.tga");
  rockTex = m_targas->add("textures/rocky256.tga");
  stoneTex = m_targas->add("textures/walls064.tga");

  // init and place objects in world
  m_floor->vinit(m_targas->getPtr(marbTex), m_targas->getWidth(marbTex), 
                 m_targas->getHeight(marbTex));
  m_dome->vinit(m_targas->getPtr(stoneTex), m_targas->getWidth(stoneTex),
                m_targas->getHeight(stoneTex),
                m_targas->getPtr(rockTex), m_targas->getWidth(rockTex),
                m_targas->getHeight(rockTex));
  m_lamp->vinit();
  m_endTable->vinit();
  m_endTable->setPosition(Vec3D(-8.0f, 0.0f, -2.0f), 10.0f);
  m_torus->vinit();
  m_torus->setPosition(Vec3D(1.0f, 0.02f, -5.0f));
  m_pawn->vinit();
  m_pawn->setPosition(Vec3D(-5.8f, 0.0f, -4.1f));
    
  // insert into view list

  glEnable(GL_LIGHTING);           

  // set light0
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);


  glEnable(GL_LIGHT0);

  glEnable(GL_FOG);                       // remember to disable for shadows
  glFogf(GL_FOG_DENSITY, 0.033f);
  glFogfv(GL_FOG_COLOR, fogColor); 
  glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);
  

  glEnable(GL_TEXTURE_2D);

  // create some boring walls that should be replaced later
  createWalls(m_targas->getPtr(wallTex), m_targas->getWidth(wallTex),
    m_targas->getHeight(wallTex));

  return 1;
}

// ********************************************************************
// destroy the world
void World::vdestroy(void)
{
  m_floor->vdestroy();
  m_dome->vdestroy();
  m_lamp->vdestroy();
  m_endTable->vdestroy();
  m_torus->vdestroy();
  m_pawn->vdestroy();
  return;
}

// *********************************************************************
// render the scene
void World::vrender(void)
{
  Vec3D camPos = m_cam->getPosition();
  Vec3D lookAt = m_cam->getLookAt();


  gluLookAt(camPos[0], camPos[1], camPos[2], 
              lookAt[0], lookAt[1], lookAt[2], 
              0.0f, 1.0f, 0.0f );

  // draw the shadows first
  //   used tutorial and adjusted as explained in the text (Hawkins 470).
  glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
    setShadowMatrix();

      // set up the stencil buffer so that we only draw the shadow
      // on the reflecting surface
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_ALWAYS, 3, 0xFFFFFFFF);
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
      m_floor->vrender();
      glStencilFunc(GL_LESS, 2, 0xFFFFFFFF);
      glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
      glEnable(GL_POLYGON_OFFSET_FILL);

      // draw the shadow as half-blended black
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_FOG);
      glColor4f(0.0, 0.0, 0.0, 0.5);
  
      // project the objects through the shadow matrix
      glMultMatrixf(m_shadowMatrix);
      m_dome->vrender();
      m_lamp->vrender();
      m_endTable->vrender();
      m_torus->vrender();
      m_pawn->vrender();
      glDisable(GL_BLEND);
      glEnable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_FOG);
      glDisable(GL_POLYGON_OFFSET_FILL);
      glDisable(GL_STENCIL_TEST);
  glPopMatrix();

  // now draw the real objects
  glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
    glCallList(m_dList);   // put in the boring walls 
  
    //the floor gets drawn in the shadows section
    m_dome->vrender();
    m_lamp->vrender();
    m_endTable->vrender();
    m_torus->vrender();
    m_pawn->vrender();
  glPopMatrix();

  return;
}

// ********************************************************************
// do collision tests with camera
int World::camCollisionTest(const Vec3D &pos, const scalarT &rad)
{
  int result = -1;
  Vec3D camPos = pos;
  float camRad = rad;
  
  // check walls
  if ( (pos[0] + rad < 15.0f && pos[0] - rad > -15.0f) &&
       (pos[2] + rad < 15.0f && pos[2] - rad > -15.0f) )
  {
    if (m_dome) result = m_dome->vcol(camPos, camRad);
    if (result != 1 && m_lamp) result = m_lamp->vcol(camPos, camRad);
    if (result != 1 && m_endTable) result = m_endTable->vcol(camPos, camRad);
    if (result != 1 && m_torus) result = m_torus->vcol(camPos, camRad);
    if (result != 1 && m_pawn) result = m_pawn->vcol(camPos, camRad);
  }
  else result = 1;  // wall collision

  return result;
}

// *****************************************************************
// make some boring walls and ceiling too
void World::createWalls(unsigned char *ptr, GLuint wd, GLuint ht)
{
  float ceilingAmbient[] = { 0.08f, 0.08f, 0.08f, 1.00f };
  float ceilingDiff[]    = { 0.32f, 0.26f, 0.24f, 1.00f };
  float ceilingColor[]   = { 0.30f, 0.20f, 0.20f, 1.00f };
  float wallAmbient[]    = { 0.20f, 0.20f, 0.20f, 1.00f };
  float wallDiff[]       = { 1.00f, 1.00f, 1.00f, 1.00f };
  float texHt = 6.6f;

  m_texImage1 = ptr;

  // setup texture parameters
  glGenTextures(1, &m_texID);
  glBindTexture(GL_TEXTURE_2D, m_texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, wd, ht, GL_RGB, 
                    GL_UNSIGNED_BYTE, m_texImage1);
  
  m_dList = glGenLists(1);
  glNewList(m_dList, GL_COMPILE);

  glPushMatrix();                  
  glColor4fv(ceilingColor);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ceilingAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, ceilingDiff);
  glBindTexture(GL_TEXTURE_2D, m_texID);
  glBegin(GL_QUADS);
    // ceiling

    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-15.0f, 6.6f,  15.0f);
    glVertex3f(-15.0f, 6.6f, -15.0f);
    glVertex3f( 15.0f, 6.6f, -15.0f);
    glVertex3f( 15.0f, 6.6f,  15.0f);

    glColor4f(0.8f, 0.75f, 0.74f, 1.0f);
    glMaterialfv(GL_FRONT, GL_AMBIENT, wallAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wallDiff);
    
 // glEnd();

  //glBegin(GL_QUADS);
    // far wall
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, texHt);
    glVertex3f(-15.0f, 6.6f, -15.0f);
    glTexCoord2f(0.0, 0.0f);
    glVertex3f(-15.0f, 0.0f, -15.0f);
    glTexCoord2f(15.0f, 0.0f);
    glVertex3f(15.0f, 0.0f, -15.0f);
    glTexCoord2f(15.0f, texHt);
    glVertex3f(15.0f, 6.6f, -15.0f);
    // left wall
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, texHt);
    glVertex3f(-15.0f, 6.6f, 15.0f);
    glTexCoord2f(0.0, 0.0f);
    glVertex3f(-15.0f, 0.0f, 15.0f);
    glTexCoord2f(15.0f, 0.0f);
    glVertex3f(-15.0f, 0.0f, -15.0f);
    glTexCoord2f(15.0f, texHt);
    glVertex3f(-15.0f, 6.6f, -15.0f);
    // behind wall
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, texHt);
    glVertex3f(15.0f, 6.6f, 15.0f);
    glTexCoord2f(0.0, 0.0f);
    glVertex3f(15.0f, 0.0f, 15.0f);
    glTexCoord2f(15.0f, 0.0f);
    glVertex3f(-15.0f, 0.0f, 15.0f);
    glTexCoord2f(15.0f, texHt);
    glVertex3f(-15.0f, 6.6f, 15.0f);
    // right wall
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, texHt);
    glVertex3f(15.0f, 6.6f, -15.0f);
    glTexCoord2f(0.0, 0.0f);
    glVertex3f(15.0f, 0.0f, -15.0f);
    glTexCoord2f(15.0f, 0.0f);
    glVertex3f(15.0f, 0.0f, 15.0f);
    glTexCoord2f(15.0f, texHt);
    glVertex3f(15.0f, 6.6f, 15.0f);
  glEnd();

  glPopMatrix();
  glEndList();
  return;
}

// ********************************************************
// getShadowMatrix
int World::setShadowMatrix(void)
{
  m_shadowMatrix[0]  =  m_lightPosition[1];
  m_shadowMatrix[1]  =  0.0f;
  m_shadowMatrix[2]  =  0.0f;
  m_shadowMatrix[3]  =  0.0f;
  m_shadowMatrix[4]  = -m_lightPosition[0];
  m_shadowMatrix[5]  =  0.0f;
  m_shadowMatrix[6]  = -m_lightPosition[2];
  m_shadowMatrix[7]  = -1.0f;
  m_shadowMatrix[8]  =  0.0f;
  m_shadowMatrix[9]  =  0.0f;
  m_shadowMatrix[10] =  m_lightPosition[1];
  m_shadowMatrix[11] =  0.0f;
  m_shadowMatrix[12] =  0.0f;
  m_shadowMatrix[13] =  0.0f;
  m_shadowMatrix[14] =  0.0f;
  m_shadowMatrix[15] =  m_lightPosition[1];

  return 1;
}
