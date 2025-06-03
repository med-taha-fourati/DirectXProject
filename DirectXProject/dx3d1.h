// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class

// function prototypes
HRESULT initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame();    // renders a single frame
void cleanD3D(void);   // closes Direct3D and releases memory
void init_graphics(void);

// the WindowProc function prototype
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
