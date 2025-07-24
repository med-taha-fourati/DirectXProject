// DirectXProject.cpp : Définit le point d'entrée de l'application.
//
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include "framework.h"
#include "DirectXProject.h"
#include "dx3d1.h"
#include "AudioHandler.cpp"
#include <vector>
#include <iostream>
#include <stdio.h>

//#include "BarDefinitions.h"
//#include "BarDefinitions.cpp"
//#include "SpriteLoader.h"
//#include "gamesprite.h"
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define MAX_LOADSTRING 100
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
    DWORD color;    // from the D3DFVF_DIFFUSE flag
};

//CUSTOMVERTEX vertices[][4] =
//{
//    // x     y       z     rhw   color
//    {{ 20.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 20.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 44.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 44.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }},
//
//    {{ 64.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 64.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 88.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 88.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }},
//
//    {{ 108.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 108.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 132.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 132.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }},
//
//    {{ 152.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 152.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 176.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 176.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }},
//
//    {{ 196.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 196.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 220.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 220.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }} ,
//
//    {{ 240.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 240.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 264.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 264.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }},
//
//    {{ 284.0f, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 284.0f, 20.f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 308.0f, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) },
//    { 308.0f, 20.0f, 1.f, 1.0f, D3DCOLOR_XRGB(0, 255, 0) }}
//};

struct CUSTOMVERTEX_STRUCT {
    CUSTOMVERTEX vertexes[4];
};

// temporary success
// this function creates bars for you
std::vector<CUSTOMVERTEX_STRUCT> CreateBars(uint32_t number, float height) {
    std::vector<CUSTOMVERTEX_STRUCT> vector = std::vector<CUSTOMVERTEX_STRUCT>();
    constexpr float starting = 20.f;
    constexpr float width = 24.f;
    float counter = starting;
    float calc_height = ((530.f - height) >= 530.f) ? 530.f : 530.f - height;
    for (uint32_t i = 0; i < number; i++) {
        CUSTOMVERTEX vertex[4] = {
                { counter, 530.f, 1.f, 1.0f, D3DCOLOR_XRGB(255, 255, 0)},
                { counter, 20.f + calc_height, 1.f, 1.0f, D3DCOLOR_XRGB(255, 255, 0) },
                { counter + width, 530.0f, 1.f, 1.0f, D3DCOLOR_XRGB(255, 255, 0)},
                { counter + width, 20.0f + calc_height, 1.f, 1.0f, D3DCOLOR_XRGB(255, 255, 0) }
        };
        CUSTOMVERTEX_STRUCT v_struct;
        v_struct.vertexes[0] = vertex[0];
        v_struct.vertexes[1] = vertex[1];
        v_struct.vertexes[2] = vertex[2];
        v_struct.vertexes[3] = vertex[3];
        
        vector.push_back(v_struct);
        counter += width + starting;
    }

    return vector;
}

void ModifyBar(std::vector<CUSTOMVERTEX_STRUCT>* vertices, uint32_t index, float height) {
    float calc_height = ((530.f - height) >= 530.f) ? 530.f : 530.f - height;
    vertices->at(index).vertexes[1].y = 20.f + calc_height;
    vertices->at(index).vertexes[3].y = 20.f + calc_height;
}

float height = 200;
std::vector<CUSTOMVERTEX_STRUCT> vertices = CreateBars(15, height);
int i; // loop de loop
LPDIRECT3DVERTEXBUFFER9 v_buffer;
// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale
//SpriteLoader* spL;
// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LPCWSTR hr;
    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioMeterInformation* pMeterInfo = NULL;
    if (hPrevInstance) return 0;
    hr = DeviceEnum();
    MessageBox(nullptr, hr, L"error", MB_OK);
    
    // TODO: Placez le code ici.

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECTXPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTXPROJECT));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        
       
    }

    cleanD3D();

    return (int) msg.wParam;
}



//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
// window properties (win32)
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = nullptr;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)COLOR_WINDOW;
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = nullptr;

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      20, 20, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   initD3D(hWnd);
   ShowWindow(hWnd, nCmdShow);
   init_graphics();
   
   
   //cleanD3D();
   //GameSprite *gs = new GameSprite();
   //gs->Init(hWnd, true);
   //spL = new SpriteLoader(100,100);
   //spL->InitSprite(gs->device, "simpleimage.jpg", 236,236);   
   return TRUE;
   //delete spL;
   //delete gs;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    wchar_t x[32];
    switch (message)
    {
    case WM_PAINT:
        render_frame();
        break;
    case WM_DESTROY:
        PostQuitMessage(2);
        break;
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
        swprintf_s(x, L"%c\n", wParam);
        OutputDebugString(x);
        switch (*x) {
        case 'W':
            height += 10.f;
            OutputDebugString(L"Added 10\n");
            break;
        case 'S':
            height -= 10.f;
            OutputDebugString(L"Reduced 10\n");
            break;
        }

        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// this function initializes and prepares Direct3D for use
HRESULT initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

    D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
    d3dpp.Windowed = TRUE;    // program windowed, not fullscreen
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
    d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D

    // create a device class using this information and information from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3ddev);

    if (d3ddev == nullptr) {
        MessageBox(hWnd, L"error: creating a window resulted in nothing", L"error", MB_OK);
        return 0;
    }
    return 1;
}

/*
* In order to render a frame the buffer has to be locked and unlocked to sort of "save"
* changes
*/
constexpr float pi = 3.14f;
int counter = 1;

void render_frame()
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3ddev->BeginScene();    // begins the 3D scene
    d3ddev->SetFVF(CUSTOMFVF);

    // select the vertex buffer to display
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

    VOID* pVoid;    // the void pointer

    v_buffer->Lock(0, 0, (void**)&pVoid, 0);    // lock the vertex buffer

    if (counter > 6) counter = 0;
    counter++;

    // copy the vertex buffer to the back buffer
    for (i = 0; i < vertices.size(); i++) {
        ModifyBar(&vertices, i, height * std::sinf(pi / (i+1)*counter));

        d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
        v_buffer->Release();
    }
    //bool v = vertices.empty();
    
    
    wchar_t buf[32];
    swprintf_s(buf, L"%f\n", height);
    //OutputDebugString(buf);

    memcpy(pVoid, static_cast<const void*>(vertices.data()), vertices.size() * sizeof(vertices.at(i)));    // copy the vertices to the locked buffer
    v_buffer->Unlock();

    d3ddev->EndScene();    // ends the 3D scene
    d3ddev->Present(NULL, NULL, NULL, NULL);
    
}

void cleanD3D(void)
{
    v_buffer->Release();
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D
}
// create three vertices using the CUSTOMVERTEX struct built earlier

//std::vector<CUSTOMVERTEX[]> v_vertices;

void init_graphics(void)
{

    // create the vertex and store the pointer into v_buffer, which is created globally
    d3ddev->CreateVertexBuffer(vertices.size() * sizeof(vertices.at(i)),
        0,
        CUSTOMFVF,
        D3DPOOL_MANAGED,
        &v_buffer,
        NULL);

    VOID* pVoid;    // the void pointer

    v_buffer->Lock(0, 0, (void**)&pVoid, 0);    // lock the vertex buffer
    memcpy(pVoid, static_cast<const void*>(vertices.data()), vertices.size() * sizeof(vertices.at(i)));    // copy the vertices to the locked buffer
    v_buffer->Unlock();    // unlock the vertex buffer*/
}












//gei bg
/*int g = 255;
    int b = 0;
    int i = 0;
    while (true) {
        if (i == 0 && b == 0 && g == 255) {
            for (i = 0; i < 255; i++) {
                g--;
                d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(i, g, b), 1.0f, 0);
                d3ddev->BeginScene();    // begins the 3D scene
                d3ddev->EndScene();    // ends the 3D scene
                d3ddev->Present(NULL, NULL, NULL, NULL);
            }
        }
        if (i == 255 && b == 0 && g == 0) {
            for (b = 0; b < 255; b++) {
                i--;
                d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(i, g, b), 1.0f, 0);
                d3ddev->BeginScene();    // begins the 3D scene
                d3ddev->EndScene();    // ends the 3D scene
                d3ddev->Present(NULL, NULL, NULL, NULL);
            }
        }
        if (i == 0 && b == 255 && g == 0) {
            for (g = 0; g < 255; g++) {
                b--;
                d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(i, g, b), 1.0f, 0);
                d3ddev->BeginScene();    // begins the 3D scene
                d3ddev->EndScene();    // ends the 3D scene
                d3ddev->Present(NULL, NULL, NULL, NULL);
            }
        }// displays the created frame
    }*/

///filler code///