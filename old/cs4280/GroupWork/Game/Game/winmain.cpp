//						Learning Team C
//						Members:
//							Lorin Kartchner
//							Todd William Brown
//							Royce Judd
//							Josh Braegger
//							Jeremiah Stephenson
//                      CS 4280 � 7:30 pm
//                      Section 4 Learning Team Assignment
//                      Mr. Rague
//                      Due: 03/28/06
//                      Version: 1.0
// -----------------------------------------------------------------
// Creates a scene and allows for a camera to move through it.
// A grid can be turned on and off to help notice movement
// -----------------------------------------------------------------

// -----------------------------------------------------------------
//   Change Control Section
// -----------------------------------------------------------------

//		Team Member: Lorin Kartchner
//		Version 1.0
//		Terrain texture changes, background music changes, rocket recoil, rocket sfx,
//			fixed escape/quit issue, fixed collision issues
//
//		Team Member: Todd William Brown
//		Version 1.0
//		Radar map, enemy list
//
//		Team Member: Royce Judd
//		Version 1.0
//		Enemy models, "wasd" keys, "plasma" effect on explosion, added sounds
//
//		Team Member: Josh Braegger
//		Version 1.0
//		Frames per second, repaired audio importing issues, a few minor bug fixes
//
//		Team Member: Jeremiah Stephenson
//		Version 1.0
//		Enemy models



/////////////////////////////////////////////////////////////////////////////


/*****************************************************************************************************************
**
**	OpenGL Game Development
**	winmain.cpp
**
**	
**	Written by: Brian Rague
**	Date:		July 20, 2005
**
*****************************************************************************************************************/
#define WIN32_LEAN_AND_MEAN
/* Headers *******************************************************************************************************/
#include <windows.h>
#include <gl/Gl.h>
#include <gl/Glu.h>

#include "OGLWindow.h"
// Begin - Phase 17
#include "HiResTimer.h"
// End - Phase 17

#include <cstdio>

using namespace std;

bool exiting = false;
long windowWidth = 800;
long windowHeight = 600;
long windowBits = 32;
bool fullscreen = false;
HDC hDC;

OGLWindow * g_glRender = NULL;

void SetupPixelFormat(HDC hDC)
{
    int pixelFormat;

    PIXELFORMATDESCRIPTOR pfd =
    {   
        sizeof(PIXELFORMATDESCRIPTOR),  // size
            1,                          // version
            PFD_SUPPORT_OPENGL |        // OpenGL window
            PFD_DRAW_TO_WINDOW |        // render to window
            PFD_DOUBLEBUFFER,           // support double-buffering
            PFD_TYPE_RGBA,              // color type
            32,                         // prefered color depth
            0, 0, 0, 0, 0, 0,           // color bits (ignored)
            0,                          // no alpha buffer
            0,                          // alpha bits (ignored)
            0,                          // no accumulation buffer
            0, 0, 0, 0,                 // accum bits (ignored)
            16,                         // depth buffer
            0,                          // no stencil buffer
            0,                          // no auxiliary buffers
            PFD_MAIN_PLANE,             // main layer
            0,                          // reserved
            0, 0, 0,                    // no layer, visible, damage masks
    };

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);
}

/* WndProc *******************************************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;
	static HDC hDC;
	int height, width;
	int x, y;

	switch (msg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		SetupPixelFormat(hDC);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		return 0;

	case WM_DESTROY:
	case WM_CLOSE:
	case WM_QUIT:
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		height = HIWORD(lParam);
		width = LOWORD(lParam);

		g_glRender->SetupProjection(width, height);
		return 0;

	case WM_KEYDOWN:
		int fwKeys;
		LPARAM keyData;

		fwKeys = (int)wParam;
		keyData = lParam;

		switch(fwKeys)
		{
		case VK_ESCAPE:
			if (MessageBox(NULL, "Do you want to quit?", "Quit?", MB_ICONQUESTION|MB_YESNO)==IDYES) 
			{	
				wglMakeCurrent(hDC, NULL);
				wglDeleteContext(hRC);
				PostQuitMessage(0);
				exit(0);  //kill game
			}
			
			break;
		default:
			// Begin - Phase 9
			g_glRender->OnKeyDown((int)wParam);
			// End - Phase 9
			break;
		}

		return 0;
		//Begin - Phase 6
	case WM_MOUSEMOVE:
		x = g_glRender->mouseX = g_glRender->GetMouseX(lParam);
		y = g_glRender->mouseY = g_glRender->GetMouseY(lParam);
		g_glRender->OnMouseMove(x, y);
		return 0;
		//End - Phase 6
		// Begin - Phase 16
	case WM_LBUTTONDOWN:		// left mouse button
		g_glRender->OnMouseDownL(g_glRender->GetMouseX(lParam), g_glRender->GetMouseY(lParam));
		break;
		// End - Phase 16

	default:
		break;
	}
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

/* WinMain *******************************************************************************************************/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevious, LPSTR lpCmdString, int CmdShow)
{

	WNDCLASSEX	wc;
	HWND		hwnd;
	MSG			msg;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		windowRect;

	// New Phase 3
	char str[700];

	g_glRender = new OGLWindow;

	windowRect.left=0L;
	windowRect.right=(long)windowWidth;
	windowRect.top=0L;
	windowRect.bottom=(long)windowHeight;

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor			= LoadCursor (NULL, IDC_ARROW);
	wc.hIcon			= LoadIcon (NULL, IDI_APPLICATION);
	wc.hIconSm			= LoadIcon (NULL, IDI_WINLOGO);
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WndProc;
	wc.lpszClassName	= "ShooterGame";
	wc.lpszMenuName		= NULL;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wc))
	{
		MessageBox (NULL,"Error: Cannot Register Class","ERROR!",MB_OK);
		return (0);
	}

	// Start - Phase 3
	sprintf_s(str, "Welcome to Shooter Game!\n\nKeys:\n<Up Arrow> - Forward\n<Down Arrow> - Backward\n<Left Arrow> - Left Strafe\n<Right Arrow> - Right Strafe");
	strcat_s(str, "\nOr you can use W,S,A,D.\n\nNumbers 1-5 change the terrain\n\nNumbers 6-8 change the background music");
	strcat_s(str, "\n\nM toggles the minimap");
	strcat_s(str, "\n\nf toggles Frames Per Second");
	strcat_s(str, "\n\nLeft mouse button fires rockets.\nMove mouse to look around.\n\n");
	strcat_s(str, "Numpad '+' - Increase mouse sensitivity\nNumpad '-' - Decrease mouse sensitivity\n\n");
	strcat_s(str, "Created for WSU CS4280\nby Brian Rague.\nVersion 1.5 done by Group C");
		
	MessageBox(NULL, str, "Welcome!", MB_OK);

	sprintf_s(str, "Shooter Game\n\n\nYou have 5 minutes to shoot all of the enemies on the terrain using your searching skills\n");
	strcat_s(str, "and your rockets. Can you do it?");
	MessageBox(NULL, str, "Welcome!", MB_OK);

		
	if (MessageBox(NULL, "We are going to run in 800x600x32 mode, would you like to run in fullscreen?", "Fullscreen?", MB_YESNO) == IDYES)
		fullscreen = true;
	else
		fullscreen = false;


	// End - Phase 3



	if (fullscreen)                             // fullscreen?
    {
        DEVMODE dmScreenSettings;                   // device mode
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings); 
        dmScreenSettings.dmPelsWidth = windowWidth;         // screen width
        dmScreenSettings.dmPelsHeight = windowHeight;           // screen height
        dmScreenSettings.dmBitsPerPel = windowBits;             // bits per pixel
        dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        // 
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // setting display mode failed, switch to windowed
            MessageBox(NULL, "Display mode failed", NULL, MB_OK);
            fullscreen = FALSE; 
        }
    }

    if (fullscreen)                             // Are We Still In Fullscreen Mode?
    {
        dwExStyle=WS_EX_APPWINDOW;                  // Window Extended Style
        dwStyle=WS_POPUP;                       // Windows Style
        ShowCursor(FALSE);                      // Hide Mouse Pointer
    }
    else
    {
        dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window Extended Style
        dwStyle=WS_OVERLAPPEDWINDOW;                    // Windows Style
    }

    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size



	hwnd = CreateWindowEx(NULL, "ShooterGame", "Shooting Game Application", dwStyle,
							0, 0, 
							windowRect.right - windowRect.left, 
							windowRect.bottom - windowRect.top,
							NULL, NULL, hInstance, NULL);
	
	if (!hwnd) 
	{
		MessageBox (NULL,"Error: Failed to Create Window","ERROR!",MB_OK);
		return (0);
	}

	hDC = GetDC(hwnd);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	// Phase 17 - Begin  Set up HiResTimer
	CHiResTimer * timer = new CHiResTimer;
	timer->Init();
	// Phase 17 - End
	ShowCursor(FALSE);
	g_glRender->Init();

	while (1)
	{
		// Phase 17 - change lists or Prepare and Render calls
		g_glRender->Prepare();
		g_glRender->Render(timer->GetElapsedSeconds(1));
		// Phase 17 - End
		SwapBuffers(hDC);

		if (PeekMessage (&msg, hwnd, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) 
				break;
			TranslateMessage(&msg);
			DispatchMessage (&msg);
			
		}
	}
	// Begin - Phase 5
	g_glRender->Shutdown();
	// End - Phase 5
	delete g_glRender;

	if (fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	return static_cast<int>(msg.wParam);
}