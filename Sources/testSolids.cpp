// main.cpp
/*
  ESC to quit
  A   toggle antialias
  R	  toggle rotating
	P   toggle wireframe
*/
#define WIN32_LEAN_AND_MEAN                  // trim the excess fat from Windows
#define WIN32_EXTRA_LEAN

#include <windows.h>					// standard windows app include
										// can also use windowsx.h
#include <winuser.h>					// windows constants
#include <GL/gl.h>						// standard OpenGL include
#include <GL/glu.h>						// OpenGL utilities
#include <GL/glaux.h>					// auxilliary functions
#include "TgaList.h"
//#include "Entity.h"
#include "RockCube.h"
//#include "Pyramid.h"
#include "Floor.h"
//#include "Flag.h"
#include "DS/dsObjects.h"

#define WND_CLASS_NAME  "MyOpenGLClass"

// **** globals *****
float g_angle = 0.0f;					// current angle of rotating triangle 
float g_xrot = 0.0f;
float g_yrot = 1.0f;
float g_zrot = 0.0f;
HDC   g_HDC;							// global device context
HGLRC g_HRC;							// global rendering context
HWND  g_hwnd = NULL;					// window handle
bool  g_isFullScreen = FALSE;            // toggle
bool  g_rotate = FALSE;
HINSTANCE g_hInstance;                  // application instance

TgaList *gTargas = NULL;
GLuint g_test1, g_test2, g_test3;   // display lists IDs
Floor *gFloor = NULL;

// the size of the window
GLuint g_width, g_height;

// prototypes
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void CleanUp(void);
BOOL DisplayScene();
BOOL InitializeScene();
BOOL KillWindow();
GLvoid ResizeScene(GLsizei width, GLsizei height);
BOOL SetupWindow(char *title, int width, int height, int bits, bool isFullScreen);


// ***************************
// the application entry point

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg;							// message
	bool done;							// flag for when app is done

	// create our window and set up OpenGL
  SetupWindow("CS447-Textures", 800, 600, 32, FALSE);

	done = false;                    // init for loop

	// main message loop.     note to use PeekMessage not GetMessage
	while (!done)
	{
		// use Peek because GetMessage halts app
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	    // check for a message
    {
		  if (msg.message == WM_QUIT)
		  {
			  done = true;             // if receive quit then quit
		  }
		  else
		  {
		    TranslateMessage(&msg);				// translate to event queue
		    DispatchMessage(&msg);				// dispatch to event queue
      }
		}
    else
    {
      DisplayScene();                       // render current scene
      SwapBuffers(g_HDC);					// bring back buffer to front
    }
	}
	KillWindow();                               // cleanup

	return msg.wParam;

} 

// **********************************
// WndProc 
// windows procedure event handler
// ********************************
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;							// rendering context
	static HDC hDC;                             // device context (p42)
	//int width, height;							// window width, height
	static BOOL antialias = true;              // control antialiasing
	static BOOL wireFrame = false;
	
	if (antialias)
	{
        glClear(GL_COLOR_BUFFER_BIT);      // redbook, p183
				glEnable(GL_BLEND);
				glEnable(GL_POLYGON_SMOOTH);
	}
	switch(message)
	{
		case WM_CREATE:                         // window is being created
			// see p55 bottom
			// don't need since already created
			return 0;
			break;
		case WM_CLOSE:							// window is closing
			// deselect rendering context and delete it
			wglMakeCurrent(hDC, NULL);
			wglDeleteContext(hRC);

			// send WM_QUIT to message queue
			CleanUp();
			PostQuitMessage(0);
			return 0;
			break;
		case WM_SIZE:
			// if window size changes then becomes distorted, rescale to
			// fit in window (p56)
			// update perspective with new width and height
            ResizeScene(LOWORD(lParam), HIWORD(lParam));
            return 0;
			break;
		case WM_CHAR:
		  switch (toupper(wParam))
		  {
		    case VK_ESCAPE:
			  // send WM_QUIT to message queue
			  CleanUp();
			  PostQuitMessage(0);
			  return 0;
			  break;
			case 'A':
              // toggle antialiasing
              antialias = !antialias;

			  // if just turned on then enable
			  if (antialias)
			  {
          glClear(GL_COLOR_BUFFER_BIT);      // redbook, p183
				  glEnable(GL_BLEND);
				  glEnable(GL_POLYGON_SMOOTH);

			  }
			  else
			  {
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				   glDisable(GL_BLEND);
				   glDisable(GL_POLYGON_SMOOTH);
			  }
			  return 0;
			  break;
			case 'P':
				// toggle wireframe
				wireFrame = !wireFrame;
				if (wireFrame) glPolygonMode(GL_FRONT, GL_LINE);
				else glPolygonMode(GL_FRONT, GL_FILL);
				break;
			case 'R':
			  // toggle rotating
			  g_rotate = !g_rotate;
			  return 0;
			  break;
			case 'X':
			  // set rotate on x-axis
			  g_xrot = 1.0f;
			  g_yrot = 0.0f;
			  g_zrot = 0.0f;
			  return 0;
			  break;
			case 'Y':
			  // set rotate on y-axis
			  g_xrot = 0.0f;
			  g_yrot = 1.0f;
			  g_zrot = 0.0f;
			  return 0;
			  break;
			default:
				break;
		  }
		default:
			break;
	}
	// return any remaining message back
	return DefWindowProc(hwnd, message, wParam, lParam);
} 

// ***********************************************************
// CleanUp

void CleanUp(void)
{
  //gTestObj->vdestroy();
  //delete gTestObj;
  //testObj = NULL;
  gFloor->vdestroy();
	if (gTargas) delete gTargas;
  return;
} 

// ***********************************************************
// DisplayScene()
// Actually draw the scene. This will be called every frame.
// *********************************************************
BOOL DisplayScene()
{
  float matAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };           // amount reflected
  float matDiff[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  float matSpec[] = { 0.9f, 0.9f, 0.9f, 1.0f };

  float lightPosition[] = { 3.0f, 6.0f, 12.0f, 0.0f };
  //float lightPosition[] = { 3.0f, 300.0f, 500.0f, 0.0f };
  // set cam back (+z) 
  glLoadIdentity();					         // reset modelview matrix
  
  //gluLookAt(0.0f, 50.4f, 300.0f, 0.0f, 55.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  gluLookAt(0.0f, 2.5f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
 
  // clear screen to black bg and clear depth buffer
  glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
  

  //glPushMatrix();
    if (g_rotate)
    {
      g_angle = g_angle + 0.1f;				     // increase rot angle counter
      if (g_angle >= 360.0f) g_angle = 0.0f;
    }
    
  
  //glPopMatrix();
 
  glPushMatrix();
    //glLoadIdentity();
    //glTranslatef(-10.0f, 1.0f, 0.0f);
    glRotatef(g_angle, g_xrot, g_yrot, g_zrot);      // rotate 
    glCallList(g_test1);
  glPopMatrix();
  

 /*
  glPushMatrix();
    //glLoadIdentity();
   // glTranslatef(0.0f, 1.0f, 1.0f);
    glCallList(g_test2);
  glPopMatrix();
 /*
  glPushMatrix();
    //glLoadIdentity();
    glTranslatef(10.0f, 1.0f, 0.0f);
    glCallList(g_test3);
  glPopMatrix();
 */
  //gFloor->vrender();

  glFlush();
  return true; 
}
 

 
/****************************************************************************
 InitializeScene()

 Sets up everything for our scene that only needs to be done once.
*****************************************************************************/
BOOL InitializeScene()
{
  int rockTex = -1;
	int pyramidTex = -1;
  int marbTex = -1;
  //int mortTex = -1;
  GLuint texID = 0;

  // set lighting variables
  float ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };         // ambient element of light
  float diffuseLight[] = { 0.85f, 0.85f, 0.85f, 1.0f };      // diffuse element of light
  float specLight[] = { 0.7f, 0.7f, 0.7f , 1.0f };
  //float ambientLightSpot[] = { 0.7f, 0.6f, 0.5f, 1.0f };
  //float diffuseLightSpot[] = { 0.66f, 0.66f, 0.66f, 1.0f };
  //float spotPosition[] = { 4.0f, 5.0f, 200.0f, 0.0f };
  //float spotDirection[] = { -4.0f, -4.0f, -2.0f };


  // load textures
  gTargas = new TgaList(6);
	marbTex = gTargas->add("textures/marbrose.tga");
	pyramidTex = gTargas->add("textures/pyramain256.tga");
  rockTex = gTargas->add("textures/rocky256.tga");
/*
  glGenTextures(1, &texID);                               
  glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    //              GL_NEAREST_MIPMAP_LINEAR);
	                  GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // make the mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 gTargas->getPtr(marbTex));
*/
  // start hidden surface removal
  glEnable(GL_DEPTH_TEST); 

  //glEnable(GL_SMOOTH);         // ??
  // hide back faces of polys
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_LIGHTING);           

  // set light1
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specLight);
  
  
  // set light1
  //glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLightSpot);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLightSpot);
  //glLightfv(GL_LIGHT1, GL_POSITION, spotPosition);
  //glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);          // 60 degree cone
  //glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0f);
  //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);

  glEnable(GL_LIGHT1);
  //glEnable(GL_LIGHT1);

  glEnable(GL_TEXTURE_2D);

  gFloor = new Floor(20.0f, 20.0f);
  gFloor->vinit(gTargas->getPtr(rockTex));

  // test solids - generate lists
  
  g_test1=glGenLists(1);
  glNewList(g_test1, GL_COMPILE);
    //glColor4f(0.0f, 1.0f, 0.5f, 0.0f);
    //glBindTexture(GL_TEXTURE_2D, texID);
    int test = dsSphere();
    
    if (test == -1) MessageBox(NULL, "Unable to create Poly - DupePt", "Error", MB_OK);
    else if (test == -2) MessageBox(NULL, "Unable to create Poly - NonPlanar", "Error", MB_OK);
  glEndList();
  
  
  g_test2=glGenLists(1);
  glNewList(g_test2, GL_COMPILE);
    glColor3f(0.1f, 0.2f, 1.0f);
    dsTorus(2.0f, 0.25f, 24, 0.5f);
  glEndList();

  /*
  g_test3=glGenLists(1);
  glNewList(g_test3, GL_COMPILE);
    glColor3f(1.0f, 0.2f, 0.3f);
    dsCylinder();
  glEndList();
  */
  
  

  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_COLOR_MATERIAL);
  
  return TRUE; 
}

/****************************************************************************
 KillWindow()

 Restore the original display and dispose of the window, device context, and
 rendering context properly.
*****************************************************************************/
BOOL KillWindow()
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
} // end KillWindow()

/****************************************************************************
 Resize()

 Sets up the correct perspective based on the current screen size.
*****************************************************************************/
GLvoid ResizeScene(GLsizei width, GLsizei height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(60.0f,(GLfloat)width/(GLfloat)height,0.1f,400.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

}

/****************************************************************************
 SetupWindow()

 Creates a window and the device and rendering contexts for it.
*****************************************************************************/
BOOL SetupWindow(char *title, int width, int height, int bits, bool isFullscreen)
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
  wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);	// default icon
  wc.hIconSm        = LoadIcon(NULL, IDI_WINLOGO);		  // windows logo small icon
  wc.hCursor        = LoadCursor(NULL, IDC_ARROW);		  // default arrow
  wc.hbrBackground  = NULL; //(HBRUSH) GetStockObject(BLACK_BRUSH);   // black background
  wc.lpszMenuName   = NULL;     // no menu
  wc.lpszClassName  = WND_CLASS_NAME;
  
  // register the windows class
  if (!RegisterClassEx(&wc))
  {
    MessageBox(NULL,"Unable to register the window class", "Error", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;							// Exit And Return FALSE
  }

  // if we're in fullscreen mode, set the display up for it
  if (g_isFullScreen)
  {
    // set up the device mode structure
    DEVMODE screenSettings;
    memset(&screenSettings,0,sizeof(screenSettings));

    screenSettings.dmSize       = sizeof(screenSettings);	
    screenSettings.dmPelsWidth  = width;			// screen width
    screenSettings.dmPelsHeight = height;			// screen height
    screenSettings.dmBitsPerPel = bits;				// bits per pixel
    screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // attempt to switch to the resolution and bit depth we've selected
    if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    { 
        g_isFullScreen = FALSE;	
    }
  }

  DWORD dwExStyle;
  DWORD dwStyle;

  // if we're still in fullscreen mode, set the window style appropriately
  if (g_isFullScreen)
  {
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;			  // simple window with no borders or title bar
		ShowCursor(FALSE);            // hide the cursor for now
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
		KillWindow();
		MessageBox(NULL, "Unable to create window", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

  // get a device context
	if (!(g_HDC = GetDC(g_hwnd)))
	{
    // reset the display
		KillWindow();
		MessageBox(NULL,"Unable to create device context", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

  // set the pixel format we want (p55)
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
		KillWindow();
		MessageBox(NULL, "Can't find an appropriate pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// set pixel format to device context
  if(!SetPixelFormat(g_HDC, pixelFormat,&pfd))
	{
    // reset the display
		KillWindow();
		MessageBox(NULL, "Unable to set pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
  }

  // create the OpenGL rendering context
  if (!(g_HRC = wglCreateContext(g_HDC)))
	{
    // reset the display
		KillWindow();
		MessageBox(NULL, "Unable to create OpenGL rendering context", "Error",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

  // now make the rendering context the active one
  if(!wglMakeCurrent(g_HDC, g_HRC))
	{
    // reset the display
		KillWindow();
		MessageBox(NULL,"Unable to activate OpenGL rendering context", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

  // show the window in the forground, and set the keyboard focus to it
  ShowWindow(g_hwnd, SW_SHOW);
  SetForegroundWindow(g_hwnd);
  SetFocus(g_hwnd);

  // set up the perspective for the current screen size
  ResizeScene(width, height);

  // do one-time initialization
  
  if (!InitializeScene())
  {
    // reset the display
	  KillWindow();
	  MessageBox(NULL, "Initialization failed", "Error", MB_OK | MB_ICONEXCLAMATION);
	  return FALSE;
  }
  
  return TRUE;
} // end SetupWindow()
