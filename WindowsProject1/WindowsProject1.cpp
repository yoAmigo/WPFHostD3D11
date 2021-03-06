// WindowsProject1.cpp: アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <atlbase.h>
#include <d3d11.h>

#include "WindowsProject1.h"

#define MAX_LOADSTRING 100
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

ID3D11Device *gpD3DDevice;
ID3D11DeviceContext *gpD3DDeviceContext;
IDXGISwapChain *gpDxgiSwapChain;
ID3D11RenderTargetView *gpD3DRenderTargetView;
ID3D11VertexShader *gpD3DVertexShader;
ID3D11PixelShader *gpD3DPixelShader;
ID3D11Buffer *gpD3DBuffer;
ID3D11InputLayout *gpD3DInputLayout;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//ポリゴン頂点構造体
struct Vertex3D {
	float pos[3];	//x-y-z
	float col[4];	//r-g-b-a
};

//頂点データ(三角ポリゴン1枚)
Vertex3D gVectorData[] = {
	{{ 0.f,   0.5f, 0.5f},{0.75f, 0.5f,  0.25f, 1.f}},
	{{ 0.5f, -0.5f, 0.5f},{0.25f, 0.75f, 0.5f,  1.f}},
	{{-0.5f, -0.5f, 0.5f},{0.5f,  0.25f, 0.75f, 1.f}}
};
const int ff = sizeof(gVectorData[0].col);
const int NUM_VERTEX = sizeof(gVectorData) / sizeof(gVectorData[0]);

//頂点レイアウト
D3D11_INPUT_ELEMENT_DESC gInElementDesc[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(gVectorData[0].pos), D3D11_INPUT_PER_VERTEX_DATA, 0}
};


enum ShaderType
{
	VertexShader, PixelShader
};

HRESULT CreateShader(ShaderType type, TCHAR* csoFile, ID3D11DeviceChild** pShader);
HRESULT InitShaderAndBuffer();
HRESULT InitDevice(HWND);
VOID CleanupDevice();
void DrawD3D();

ATOM                MyRegisterClass2();
LRESULT CALLBACK    WndProc2(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化しています。
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーションの初期化を実行します:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	
	CleanupDevice();

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   InitDevice(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			DrawD3D();
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

HRESULT CreateShader(ShaderType type, TCHAR* csoFile, ID3D11DeviceChild** pShader)
{
	auto hr = E_FAIL;
	FILE *fp;
	if (_tfopen_s(&fp, csoFile, _T("rb")) != 0)
		return hr;

	fseek(fp, 0, SEEK_END);
	auto cso_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	auto cso_data = new unsigned char[cso_size];
	fread_s(cso_data, cso_size, 1, cso_size, fp);
	fclose(fp);

	switch (type)
	{
	case VertexShader:
	{
		hr = gpD3DDevice->CreateInputLayout(gInElementDesc, ARRAYSIZE(gInElementDesc), cso_data, cso_size, &gpD3DInputLayout);
		if (SUCCEEDED(hr))
		{
			ID3D11VertexShader* pVertexShader;
			hr = gpD3DDevice->CreateVertexShader(cso_data, cso_size, nullptr, &pVertexShader);
			if (SUCCEEDED(hr))
			{
				gpD3DDeviceContext->IASetInputLayout(gpD3DInputLayout);
				*(ID3D11VertexShader **)pShader = pVertexShader;
			}
		}
	}
		break;
	case PixelShader:
	{
		ID3D11PixelShader* pPixelShader;
		hr = gpD3DDevice->CreatePixelShader(cso_data, cso_size, nullptr, &pPixelShader);
		if (SUCCEEDED(hr))
			*(ID3D11PixelShader **)pShader = pPixelShader;
	}
	break;
	default:
		break;
	}

	delete[]cso_data;

	return hr;
}

HRESULT InitShaderAndBuffer()
{
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(Vertex3D)*NUM_VERTEX;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	
	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = gVectorData;
	srd.SysMemPitch = 0;
	srd.SysMemSlicePitch = 0;

	auto hr = gpD3DDevice->CreateBuffer(&bd, &srd, &gpD3DBuffer);
	if (FAILED(hr))
		return hr;

	auto strides = sizeof(Vertex3D);
	auto offsets = 0u;
	gpD3DDeviceContext->IASetVertexBuffers(0, 1, &gpD3DBuffer, &strides, &offsets);
	gpD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	TCHAR vsCso[] = _T("VertexShader.cso");
	hr = CreateShader(VertexShader, vsCso, (ID3D11DeviceChild **)&gpD3DVertexShader);
	if (FAILED(hr))
		return hr;

	gpD3DDeviceContext->VSSetShader(gpD3DVertexShader, nullptr, 0);

	TCHAR psCso[] = _T("PixelShader.cso");
	hr = CreateShader(PixelShader, psCso, (ID3D11DeviceChild **)&gpD3DPixelShader);
	if (FAILED(hr))
		return hr;

	gpD3DDeviceContext->PSSetShader(gpD3DPixelShader, nullptr, 0);

	return hr;
}

HRESULT InitDevice(HWND hWnd)
{
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0/*,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0*/
	};
	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);
	D3D_FEATURE_LEVEL featureLevel;

	RECT rc;
	GetClientRect(hWnd, &rc);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rc.right - rc.left;
	sd.BufferDesc.Height = rc.bottom - rc.top;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	auto hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &gpDxgiSwapChain, &gpD3DDevice,
		&featureLevel, &gpD3DDeviceContext);
	if (FAILED(hr))
	{
		CleanupDevice();
		return hr;
	}

	CComPtr<ID3D11Texture2D> pD3DTexture;
	hr = gpDxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&pD3DTexture));
	if (FAILED(hr))
	{
		CleanupDevice();
		return hr;
	}

	hr = gpD3DDevice->CreateRenderTargetView(pD3DTexture, nullptr, &gpD3DRenderTargetView);
	if (FAILED(hr))
	{
		CleanupDevice();
		return hr;
	}

	gpD3DDeviceContext->OMSetRenderTargets(1, &gpD3DRenderTargetView, nullptr);

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = sd.BufferDesc.Width;
	vp.Height = sd.BufferDesc.Height;
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	gpD3DDeviceContext->RSSetViewports(1, &vp);

	hr = InitShaderAndBuffer();
	if (FAILED(hr))
	{
		CleanupDevice();
		return hr;
	}

	return S_OK;
}

VOID CleanupDevice()
{
	SAFE_RELEASE(gpD3DInputLayout);
	SAFE_RELEASE(gpD3DBuffer);
	SAFE_RELEASE(gpD3DPixelShader);
	SAFE_RELEASE(gpD3DVertexShader);
	SAFE_RELEASE(gpD3DRenderTargetView);
	SAFE_RELEASE(gpDxgiSwapChain);
	SAFE_RELEASE(gpD3DDeviceContext);
	SAFE_RELEASE(gpD3DDevice);
}

float clearColor[] = { 0.f,0.f,1.f,1.f };
void DrawD3D()
{
	gpD3DDeviceContext->ClearRenderTargetView(gpD3DRenderTargetView, clearColor);
	gpD3DDeviceContext->Draw(NUM_VERTEX, 0);
	gpDxgiSwapChain->Present(0, 0);
}

ATOM MyRegisterClass2()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc2;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = nullptr;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = _T("DllWindow");
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		DrawD3D();
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


IntPtr ClassLibrary1::Class1::CreateHostedWindow(IntPtr parentHwnd, int width, int height)
{
	MyRegisterClass2();
	auto hWnd = CreateWindow(_T("DllWindow"), _T("hosted window"), WS_CHILD | WS_VISIBLE, 0, 0, width, height, (HWND)parentHwnd.ToPointer(), nullptr, nullptr, 0);
	InitDevice(hWnd);
	return IntPtr(hWnd);
}

void ClassLibrary1::Class1::DestroyHostedWindow(IntPtr hWnd)
{
	CleanupDevice();
	DestroyWindow((HWND)hWnd.ToPointer());
}

void ClassLibrary1::Class1::Effect()
{
	if (clearColor[0] == 1.f) { clearColor[0] = 0.f; clearColor[1] = 1.f; }
	else if (clearColor[1] == 1.f) { clearColor[1] = 0.f; clearColor[2] = 1.f; }
	else { clearColor[2] = 0.f; clearColor[0] = 1.f; }

	DrawD3D();
}
