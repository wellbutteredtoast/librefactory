#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

/* libs before includes */
#pragma comment(lib, "d3d8.lib")
#pragma comment(lib, "d3dx8.lib")
#pragma comment(lib, "dxguid.lib")

#include <windows.h>
#include <d3d8.h>

LPDIRECT3D8       g_pD3D    = NULL;
LPDIRECT3DDEVICE8 g_pDevice = NULL;

/* Initalize the D3D device, and other basics, rets a boolean */
bool InitDirect3D(HWND hwnd, int width, int height)
{
	g_pD3D = Direct3DCreate8(D3D_SDK_VERSION);
	if (!g_pD3D) return false;

    D3DPRESENT_PARAMETERS pp;
    ZeroMemory(&pp, sizeof(pp));

	/* Why do these need to be capitalized? why are bool and BOOL different? */
    pp.Windowed               = TRUE;
    pp.SwapEffect             = D3DSWAPEFFECT_COPY;
    pp.BackBufferFormat       = D3DFMT_R5G6B5; /* matches current desktop format */
	pp.BackBufferCount        = 1;
    pp.BackBufferWidth        = width;
    pp.BackBufferHeight       = height;
    pp.EnableAutoDepthStencil = FALSE; /* 2D, no depth buffer needed */

    HRESULT hr = g_pD3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_REF, /* reference, or software rendering */
        hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &pp,
        &g_pDevice
    );

	if (FAILED(hr))
	{
		char buf[64];
		wsprintfA(buf, "CreateDevice failed: 0x%08X", (unsigned long)hr);
		MessageBox(hwnd, buf, "D3D8 Error", MB_OK);
		return false;
	}

    return true;
}

/* Simplifies the shutdown of D3D8 */
void ShutdownDirect3D()
{
    if (g_pDevice) { g_pDevice->Release(); g_pDevice = NULL; }
    if (g_pD3D)    { g_pD3D->Release();    g_pD3D    = NULL; }
}

/* Window procedure */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
                DestroyWindow(hwnd);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/* entrypoint file, don't use main, only WinMain() ! */
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdLine, int cmdShow)
{
	/* Registering the window class */
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "LibreFactoryWindow";

	if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window registration failed!", "Error", MB_OK);
        return 1;
    }

	/* Now we make the actual window and hwnd */
	HWND hwnd = CreateWindowEx(
		0,
        "LibreFactoryWindow",       /* class name */
        "LibreFactory",             /* title bar */
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        640, 480,
        NULL, NULL,
        hInst,
        NULL
	);

	if (!hwnd)
	{
		MessageBox(NULL, "CreateWindowEx() failed!", "Error", MB_OK);
		return 1;
	}

	ShowWindow(hwnd, cmdShow);
    UpdateWindow(hwnd);

    /* Finally, init d3d8 proper */
    if (!InitDirect3D(hwnd, 640, 480))
    {
        MessageBox(hwnd, "Direct3D 8 init failed!", "D3D8 Error", MB_OK);
        return 1;
    }
	
    /* Message + render loop */
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            /* Render frame */
            g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
            g_pDevice->BeginScene();
			
            /* draw stuff when we got stuff to draw */
			
            g_pDevice->EndScene();
            g_pDevice->Present(NULL, NULL, NULL, NULL);
        }
    }

    ShutdownDirect3D();
    return (int)msg.wParam;
}
