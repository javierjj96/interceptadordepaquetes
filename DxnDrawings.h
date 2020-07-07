#include <Windows.h>
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"
#include <d3d9.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")


//variable declaration external to be able to use it in .cpp
extern IDirect3D9Ex* dx_Object;
extern IDirect3DDevice9Ex* dx_Device;
extern D3DPRESENT_PARAMETERS dx_Param;
extern ID3DXFont* dx_Font;
extern int Render();
extern int Width;
extern int Height;
extern HWND hWnd;
extern MSG Message;
extern HWND TargetWnd;
extern HINSTANCE hInstance;
extern RECT TargetRect;
extern RECT CheckWnd;
extern HINSTANCE  inj_hModule;


LRESULT CALLBACK Proc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL RegisterDLLWindowClass(wchar_t szClassName[]);
DWORD WINAPI ThreadProc(LPVOID lpParam);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved);

int DirectXxInit(HWND hWnd)
{
    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &dx_Object)))
        exit(1);

    dx_Param.BackBufferFormat = D3DFMT_A8R8G8B8;
    dx_Param.hDeviceWindow = hWnd;
    dx_Param.MultiSampleQuality = DEFAULT_QUALITY;
    dx_Param.SwapEffect = D3DSWAPEFFECT_DISCARD;
    dx_Param.Windowed = true;

    if (FAILED(dx_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_Param, 0, &dx_Device)))
        exit(1);

    D3DXCreateFont(dx_Device, 15, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Verdana", &dx_Font);

    return 0;
}

void DrawFilledBox(int x, int y, int w, int h, int r, int g, int b, int a)
{
    static ID3DXLine* pLine;

    if (!pLine)
        D3DXCreateLine(dx_Device, &pLine);

    D3DXVECTOR2 vLine[2];

    pLine->SetWidth(w);
    pLine->SetAntialias(false);
    pLine->SetGLLines(true);

    vLine[0].x = x + w / 2;
    vLine[0].y = y;
    vLine[1].x = x + w / 2;
    vLine[1].y = y + h;

    pLine->Begin();
    pLine->Draw(vLine, 2, D3DCOLOR_RGBA(r, g, b, a));
    pLine->End();
}

void DrawBox(int x, int y, int width, int height, int px, int r, int g, int b, int a)
{
    DrawFilledBox(x, y + height, width, px, r, g, b, a);
    DrawFilledBox(x - px, y, px, height, r, g, b, a);
    DrawFilledBox(x, y - px, width, px, r, g, b, a);
    DrawFilledBox(x + width, y, px, height, r, g, b, a);
}

int DrawText(char* String, int x, int y, int r, int g, int b)
{
    RECT Font;
    Font.bottom = 0;
    Font.left = x;
    Font.top = y;
    Font.right = 0;

    dx_Font->DrawTextA(0, String, strlen(String), &Font, DT_NOCLIP, D3DCOLOR_ARGB(255, r, g, b));

    return 0;
}

int DrawShadowText(char* String, int x, int y, int r, int g, int b)
{
    RECT Font;
    Font.bottom = 0;
    Font.left = x;
    Font.top = y;
    Font.right = 0;

    RECT Fonts;
    Fonts.bottom = 0;
    Fonts.left = x + 1;
    Fonts.top = y + 1;
    Fonts.right = 0;

    dx_Font->DrawTextA(0, String, strlen(String), &Fonts, DT_NOCLIP, D3DCOLOR_ARGB(255, 1, 1, 1));
    dx_Font->DrawTextA(0, String, strlen(String), &Font, DT_NOCLIP, D3DCOLOR_ARGB(255, r, g, b));

    return 0;
}
