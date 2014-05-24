// main.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.29

    Architectural Walkthrough displaying Solids Modeling. 
    Featuring solid modeling using extrusion, lathing, and cell 
    decomposition. User may walk around a room using keyboard and
    switch between wireframe and normal view.

    Requires HiResTimer.h to compile written by Dave Astle. It is
    a wrapper for querying the windows high performance timer.
	Will also run by commenting out HiResTimer.h and lines with timer

    ************************************************************
    References:

    These include tutorials, reading describing concepts, and further reading
    for future improvements for this file. See overview.doc for complete list
    for entire project.

    Hawkins, Kevin and Astle, Dave. OpenGL Game Programming. 
             2001 Prima Publishing.

    McShaffry, Mike. Game Coding Complete. 2003 Paraglyph Press.
    
*/
/* **********************************************
  Keyboard commands:

  ESC:         to quit
  M:           toggle wireframe
  Arrow keys:  movement, turning
  W:           look up
  X:           look down

* ********************************************* */
#define WIN32_LEAN_AND_MEAN             // trim libraries used
#define WIN32_EXTRA_LEAN

#include <windows.h>                    // standard windows app include
#include <winuser.h>                    // windows constants
#include <GL/gl.h>                      // standard OpenGL include
#include <GL/glu.h>                     // OpenGL utilities
#include "../Astle/HiResTimer.h"        // By Dave Astle (Hawkins 648)
#include "Vec3D.h"
#include "Mat3D.h"
#include "consts.h"
#include "World.h"
#include "Floor.h"
#include "TgaList.h"

// **** globals *****
HDC        g_HDC;                       // global device context
HGLRC      g_HRC;                       // global rendering context
HWND       g_hwnd = NULL;               // window handle
bool       g_isFullScreen = true;       // toggle
HINSTANCE  g_hInstance;                 // application instance

BOOL       g_isActive = true;           // if window active
GLboolean  g_keys[256];                 // keyboard pressing

World     *g_world;                     // this is the world
TgaList   *gTargas = NULL;

#define WND_CLASS_NAME  "CS447Project"
#define PROG_NAME       "CS447-David Shrock-2004"

// ************************************************************************
// prototypes
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, 
                         LPARAM lParam);

void cleanUp(void);           // unload the world
BOOL initialize(void);        // load the world
BOOL killWindow(void);
void mainCycle(float time);   // main activity loop

// updates deltas for change in camera
int processInput(Vec3D &deltaPos, float &deltaPitch, float &deltaYaw);

GLvoid resizeScene(GLsizei width, GLsizei height);
BOOL setupWindow(char *title, int width, int height, int bits, 
                 bool isFullScreen);


// **************************************************************************
// WinMain  - the application entry point
// based on standard Windows message loop example
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nShowCmd)
{
  CHiResTimer timer;            // timer by Dave Astle (Hawkins)
  float deltaTime = 0.005;      // get from timer
  MSG msg;                      // message
  bool done = false;            // flag for when app is done

  // create our window and set up OpenGL
  setupWindow(PROG_NAME, SCREEN_WD, SCREEN_HT, 32, g_isFullScreen);

  // do one-time initialization
  if (!initialize())
  {
    // reset the display
    killWindow();
    MessageBox(NULL, "Init failed", "Error", MB_OK | MB_ICONEXCLAMATION);
    return 0;
  }

  // the timer is used to keep movement based on real time rather than FPS.
  timer.Init();                // start timer

  // main message loop - go until quit
    /* This is a genric windows message loop. It is suggested that this can
       be optimized and is left as an excercise in Game Coding Concepts
       (McSHaffry 227)
    */
  while (!done)
  {
    // use Peek because GetMessage halts app
    if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))       // check for a message
    {
      if (msg.message == WM_QUIT)
      {
        done = true;             // if receive quit then quit
      }
      else
      {
        TranslateMessage(&msg);                  // translate to event queue
        DispatchMessage(&msg);                   // dispatch to event queue
      }
    }
    else
    {
      deltaTime = timer.GetElapsedSeconds(1);
      mainCycle(deltaTime);                      // render current scene
      
    }
  }

  cleanUp();
  killWindow();                     // close the window

  return msg.wParam;
} 

// ***********************************************************************
// WndProc 
// windows procedure event handler
// using WGL (see Hawkins 44)
// 
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static HGLRC hRC;                 // rendering context
  static HDC hDC;                   // device context
  //int width, height;              // window width, height
  static BOOL wireFrame = false;

  switch(message)
  {
    //case WM_CREATE:                             // window is being created
      // see p55 bottom
      // don't need since already created
      //return 0;
      //break;
    case WM_ACTIVATE:                            // window min or maxed
      if (!HIWORD(wParam))
      {
        // program was restored or maximized
        g_isActive = TRUE;
      }
      else
      {
        // program was minimized
        g_isActive=FALSE;
      }

      return 0;
      break;
    case WM_CLOSE:    // window is closing
      // deselect rendering context and delete it
      wglMakeCurrent(hDC, NULL);
      wglDeleteContext(hRC);

      // send WM_QUIT to message queue
      PostQuitMessage(0);
      return 0;
      break;
    case WM_SIZE:                                     // resize
      // update perspective with new width and height
      resizeScene(LOWORD(lParam), HIWORD(lParam));
      return 0;
      break;
    case WM_KEYDOWN:
      switch (toupper(wParam))
      {
        case VK_ESCAPE:
          // send WM_QUIT to message queue
          PostQuitMessage(0);
          return 0;
          break;
        case 'M':
          // toggle wireframe
          wireFrame = !wireFrame;
          if (wireFrame) glPolygonMode(GL_FRONT, GL_LINE);
          else glPolygonMode(GL_FRONT, GL_FILL);
          break;
        default:
          g_keys[wParam] = GL_TRUE;
          return 0;
          break;
      };
    case WM_KEYUP:
      g_keys[wParam] = GL_FALSE;
      return 0;
      break;

    default:
      break;
  }
  // return any other messages
  return (DefWindowProc(hwnd, message, wParam, lParam));
} 

// ***********************************************************
// cleanUp
// 
void cleanUp(void)
{
  if (g_world) 
  {
    g_world->vdestroy();
    delete g_world;
  } 
} 
 
 
// **************************************************************************
// initialize
// set up everything for the scene that only needs to be done once.
//
BOOL initialize(void)
{
  // start hidden surface removal
  glEnable(GL_DEPTH_TEST); 
  glEnable(GL_SMOOTH);
  // hide back faces of polys
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // use Gouraud shading
  glShadeModel(GL_SMOOTH);   // or flat for blocky
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE);         // redbook p183

  // get rid of funny lines in mipmaps (redbook 232)
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

  // load the world
  g_world = new World();
  g_world->vinit();

  return TRUE; 
}

// ************************************************************************
// killWindow
// Restore the original display.
// based on Tutorial (Hawkins)
BOOL killWindow()
{ 
  // restore the original display if we're in fullscreen mode
  if (g_isFullScreen)
  {
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(TRUE);
  }

  // if we have an RC, release it
  if (g_HRC)
  {
    // release the RC
    if (!wglMakeCurrent(NULL,NULL))
    {
      MessageBox(NULL, "Unable to release rendering context", "Error", MB_OK | MB_ICONINFORMATION);
    }

    // delete the RC
    if (!wglDeleteContext(g_HRC))
    {
      MessageBox(NULL, "Unable to delete rendering context", "Error", MB_OK | MB_ICONINFORMATION);
    }

    g_HRC = NULL;
  }

  // release the DC if we have one
  if (g_HDC && !ReleaseDC(g_hwnd, g_HDC))
  {
    MessageBox(NULL, "Unable to release device context", "Error", MB_OK | MB_ICONINFORMATION);
    g_HDC = NULL;
  }

  // destroy the window if we have a valid handle
  if (g_hwnd && !DestroyWindow(g_hwnd))
  {
    MessageBox(NULL, "Unable to destroy window", "Error", MB_OK | MB_ICONINFORMATION);
    g_hwnd = NULL;
  }

  // unregister our class so we can create a new one if we need to
  if (!UnregisterClass(WND_CLASS_NAME, g_hInstance))
  {
    MessageBox(NULL, "Unable to unregister window class", "Error", MB_OK | MB_ICONINFORMATION);
    g_hInstance = NULL;
  }
  return TRUE;
}

// **********************************************************************
/* mainCycle
   A walkthrough cycle - this should be called from the main message loop
   1. get elapsed time since last cycle
   2. process input
   3. calculate collisions
   3. do rendering
*/
void mainCycle(float time)
{
  Vec3D deltaPos(0.0f, 0.0f, 0.0f);                // change in camera
  float deltaPitch = 0.0f;
  float deltaYaw = 0.0f;

  if (g_isActive)                           // don't do anything if not ready
  {
    // 1. get elapsed time - we did: time

    // 2. process input, updates deltas
    processInput(deltaPos, deltaPitch, deltaYaw);
    deltaPos *= time;
    deltaPitch *= time;
    deltaYaw *= time;

    // 3. calculate collisions, send deltas to world
    g_world->moveCam(deltaPos, deltaPitch, deltaYaw);

    // 4. do rendering
    
    glLoadIdentity();                                  // reset modelview matrix
    // clear screen to black bg and clear depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // camera gluLookAt controlled by World

    // draw the world
    g_world->vrender();

    glFlush();
    SwapBuffers(g_HDC);                        // bring back buffer to front
  }
  else
  {
    WaitMessage();            // wait for something to appear in queue
  }   
}

// *************************************************************************
// process input
// find out if any keys pressed to control camera
int processInput(Vec3D &deltaPos, float &deltaPitch, float &deltaYaw)
{
  // see if the player is turning/moving (using KEY_DOWN in consts.h)

  if (KEY_DOWN(VK_LEFT))
  {
    // orient delta vector of camera left
    deltaYaw = ANGLE_FACTOR;
  }
  if (KEY_DOWN(VK_RIGHT))
  {
    // orient camera look vector right
    deltaYaw = -ANGLE_FACTOR; 
  }
  if (KEY_DOWN('W'))
  {
    // orient camera look vector up
    deltaPitch = ANGLE_FACTOR; 

  }
  if (KEY_DOWN('X'))
  {
    // orient camera look vector down
    deltaPitch = -ANGLE_FACTOR; 
  }
  if (KEY_DOWN(VK_UP))
  {
    // move cam foreward - delta is to -z
    deltaPos = Vec3D(0.0f, 0.0f, -SPEED);
  }
  if (KEY_DOWN(VK_DOWN))
  {
    // move cam back
    deltaPos = Vec3D(0.0f, 0.0f, SPEED);
  }

  return 1;
}

// **************************************************************************
// resizeScene
// Sets up the correct perspective based on the current screen size.
// based on tutorial (Hawkins 56)
GLvoid resizeScene(GLsizei width, GLsizei height)
{
  if (height==0) height = 1;              // can't divide by zero


  glViewport(0,0,width,height);           // reset The current viewport

  glMatrixMode(GL_PROJECTION);            // select the projection matrix
  glLoadIdentity();                       // reset projection matrix

  // calculate aspect ratio of window
  gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.1f,400.0f);

  glMatrixMode(GL_MODELVIEW);            // select Modelview matrix
  glLoadIdentity();                      // reset matrix
  return;
}

//***************************************************************************
// setupWindow
// Creates a window and the device and rendering contexts for it.
// based on tutorial (Hawkins 48)
BOOL setupWindow(char *title, int width, int height, int bits, bool isFullscreen)
{
  // set the global flag
  g_isFullScreen = isFullscreen;

  // get our instance handle
  g_hInstance	= GetModuleHandle(NULL);

  WNDCLASSEX  wc;    // window class

  // fill out the window class structure
  wc.cbSize         = sizeof(WNDCLASSEX);
  wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc    = WndProc;
  wc.cbClsExtra     = 0;
  wc.cbWndExtra     = 0;
  wc.hInstance      = g_hInstance;
  wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);  // default icon
  wc.hIconSm        = LoadIcon(NULL, IDI_WINLOGO);      // windows logo small icon
  wc.hCursor        = LoadCursor(NULL, IDC_ARROW);      // default arrow
  wc.hbrBackground  = NULL; //(HBRUSH) GetStockObject(BLACK_BRUSH);   // black background
  wc.lpszMenuName   = NULL;     // no menu
  wc.lpszClassName  = WND_CLASS_NAME;
  
  // register the windows class
  if (!RegisterClassEx(&wc))
  {
    MessageBox(NULL,"Unable to register the window class", "Error",
               MB_OK | MB_ICONEXCLAMATION);
    return false;           // Exit And Return FALSE
  }

  // if we're in fullscreen mode, set the display up for it
  if (g_isFullScreen)
  {
    // set up the device mode structure
    DEVMODE screenSettings;
    memset(&screenSettings,0,sizeof(screenSettings));

    screenSettings.dmSize       = sizeof(screenSettings);	
    screenSettings.dmPelsWidth  = width;                 // screen width
    screenSettings.dmPelsHeight = height;                 // screen height
    screenSettings.dmBitsPerPel = bits;                  // bits per pixel
    screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // attempt to switch to the resolution and bit depth selected
    if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN)
        != DISP_CHANGE_SUCCESSFUL)
    { 
        g_isFullScreen = false;
    }
  }

  DWORD dwExStyle;
  DWORD dwStyle;

  // if fullscreen then set style and cursor
  if (g_isFullScreen)
  {
    dwExStyle = WS_EX_APPWINDOW;
    dwStyle = WS_POPUP;           // simple window with no borders or title bar
    ShowCursor(false);            // hide the cursor for now
  }
  else
  {
    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;
  }

  // set up the window we're rendering to so that the top left corner is at (0,0)
  // and the bottom right corner is (height,width)
  RECT  windowRect;
  windowRect.left = 0;
  windowRect.right = (LONG) width;
  windowRect.top = 0;
  windowRect.bottom = (LONG) height;

  // change the size of the rect to account for borders, etc. set by the style
  AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

  // class registered, so now create our window
  g_hwnd = CreateWindowEx(dwExStyle,          // extended style
                          WND_CLASS_NAME,     // class name
                          title,              // app name
                          dwStyle |           // window style
                          WS_CLIPCHILDREN |   // required for
                          WS_CLIPSIBLINGS,    // using OpenGL
                          0, 0,               // x,y coordinate
                          windowRect.right - windowRect.left, // width
                          windowRect.bottom - windowRect.top, // height
                          NULL,               // handle to parent
                          NULL,               // handle to menu
                          g_hInstance,        // application instance
                          NULL);              // no extra params

  // see if our window handle is valid
  if (!g_hwnd)
  {
    // reset the display
    killWindow();
    MessageBox(NULL, "Unable to create window", "Error",
               MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  // get a device context
  if (!(g_HDC = GetDC(g_hwnd)))
  {
    // reset the display
    killWindow();
    MessageBox(NULL,"Unable to create device context", "Error",
               MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  // set the pixel format we want (Hawkins 55)
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),	// size of structure
    1,                              // default version
    PFD_DRAW_TO_WINDOW |            // window drawing support
    PFD_SUPPORT_OPENGL |            // OpenGL support
    PFD_DOUBLEBUFFER,               // double buffering support
    PFD_TYPE_RGBA,                  // RGBA color mode
    bits,                           // 32 bit color mode
    0, 0, 0, 0, 0, 0,               // ignore color bits, non-palettized mode
    0,                              // no alpha buffer
    0,                              // ignore shift bit
    0,                              // no accumulation buffer
    0, 0, 0, 0,                     // ignore accumulation bits
    16,                             // 16 bit z-buffer size
    0,                              // no stencil buffer
    0,                              // no auxiliary buffer
    PFD_MAIN_PLANE,                 // main drawing plane
    0,                              // reserved
    0, 0, 0 };                      // layer masks ignored
      
    GLuint  pixelFormat;

    // choose best matching pixel format
    if (!(pixelFormat = ChoosePixelFormat(g_HDC, &pfd)))
    {
      // reset the display
      killWindow();
      MessageBox(NULL, "Can't find an appropriate pixel format", "Error",
                 MB_OK | MB_ICONEXCLAMATION);
      return false;
    }

  // set pixel format to device context
  if(!SetPixelFormat(g_HDC, pixelFormat,&pfd))
  {
    // reset the display
    killWindow();
    MessageBox(NULL, "Unable to set pixel format", "Error",
               MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  // create the OpenGL rendering context
  if (!(g_HRC = wglCreateContext(g_HDC)))
  {
    // reset the display
    killWindow();
    MessageBox(NULL, "Unable to create OpenGL rendering context", "Error",
               MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  // now make the rendering context the active one
  if(!wglMakeCurrent(g_HDC, g_HRC))
  {
    // reset the display
    killWindow();
    MessageBox(NULL,"Unable to activate OpenGL rendering context", "ERROR",
               MB_OK | MB_ICONEXCLAMATION);
    return false;
  }

  // show the window in the forground, and set the keyboard focus to it
  ShowWindow(g_hwnd, SW_SHOW);
  SetForegroundWindow(g_hwnd);
  SetFocus(g_hwnd);

  // set up the perspective for the current screen size
  resizeScene(width, height);

  return true;
} 

