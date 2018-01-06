// WindowsProject2.cpp: アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <cstdio>
#include <GL\glew.h>
#include <gl\GL.h>

#include "WindowsProject2.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

HGLRC ghRc;
GLuint gProgram;
GLuint gVertexBuffer;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

const GLfloat gVertexBufferData[] = {
	-1.f, -1.f, 0.f,
	 1.f, -1.f, 0.f,
	 0.f,  1.f, 0.f
};

bool SetupPixelFormat(HDC);
GLuint CreateShaderProgram(HWND);
enum ShaderType
{
	VertexShader, FragmentShader
};

void DrawGL(HDC);
GLuint LoadShader(ShaderType, TCHAR *);
BOOL InitOpengl(HWND);
void CleanupOpengl(void);

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
    LoadStringW(hInstance, IDC_WINDOWSPROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーションの初期化を実行します:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));

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

	CleanupOpengl();

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2);
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

   if (InitOpengl(hWnd) != TRUE)
	   return FALSE;

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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
			DrawGL(hdc);
			EndPaint(hWnd, &ps);
        }
        break;
	case WM_SIZE:
	{
		auto hDc = GetDC(hWnd);
		wglMakeCurrent(hDc, ghRc);
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		auto err = glGetError();
		if (err != GL_NO_ERROR)
			MessageBox(hWnd, _T("Error Occured."), _T("Error"), 0);
		wglMakeCurrent(nullptr, nullptr);
		ReleaseDC(hWnd, hDc);
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

bool SetupPixelFormat(HDC hDc)
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	auto pf = ChoosePixelFormat(hDc, &pfd);
	if (pf == 0)
		return false;

	if (SetPixelFormat(hDc, pf, &pfd) == FALSE)
		return false;

	DescribePixelFormat(hDc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	return true;
}

GLuint CreateShaderProgram(HWND hWnd)
{

	TCHAR vertexShaderFile[] = _T("simpleVertex.shader");
	auto vsh = LoadShader(VertexShader, vertexShaderFile);
	TCHAR fragmentShaderFile[] = _T("simpleFragment.shader");
	auto fsh = LoadShader(FragmentShader, fragmentShaderFile);
	if (vsh == 0 || fsh == 0)
		return 0;

	auto program = glCreateProgram();
	if (program != 0)
	{
		glAttachShader(program, vsh);
		glAttachShader(program, fsh);
		glLinkProgram(program);
		GLint result;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint logLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
			auto *logInfo = new GLchar[logLen];
			GLint logGetLen;
			glGetProgramInfoLog(program, logLen, &logGetLen, logInfo);
			delete[]logInfo;
			glDeleteProgram(program);
			program = 0;
		}
	}

	return program;
}

void DrawGL(HDC hDc)
{
	wglMakeCurrent(hDc, ghRc);
	float backgroundColor[] = { 0.f,0.f,1.f,1.f };
	glClearColor(0.f, 0.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (gVertexBuffer != 0)
	{
		if (gProgram != 0)
			glUseProgram(gProgram);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
	}

	SwapBuffers(hDc);
	auto err = glGetError();
	if (err != GL_NO_ERROR)
		printf_s("Error(%d)\n", err);
	wglMakeCurrent(nullptr, nullptr);
}

GLuint LoadShader(ShaderType type, TCHAR *shaderFile)
{
	auto shader = type == VertexShader ? glCreateShader(GL_VERTEX_SHADER) :
		type == FragmentShader ? glCreateShader(GL_FRAGMENT_SHADER) : 0;
	if (shader == 0)
		return 0;

	FILE *fp;
	if (_tfopen_s(&fp, shaderFile, _T("rb")) != 0)
		return 0;

	fseek(fp, 0, SEEK_END);
	auto shaderFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	auto shaderSource = new unsigned char[shaderFileSize + 1];
	fread_s(shaderSource, shaderFileSize, 1, shaderFileSize, fp);
	fclose(fp);
	shaderSource[shaderFileSize] = 0;

	glShaderSource(shader, 1, (const GLchar *const *)&shaderSource, nullptr);
	glCompileShader(shader);
	delete[]shaderSource;

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		auto *logInfo = new GLchar[logLen];
		GLint logGetLen;
		glGetShaderInfoLog(shader, logLen, &logGetLen, logInfo);
		delete[]logInfo;
		glDeleteShader(shader);
		shader = 0;
	}

	return shader;
}

BOOL InitOpengl(HWND hWnd)
{
	auto hDc = GetDC(hWnd);
	if (hDc == nullptr)
		return FALSE;

	ghRc = SetupPixelFormat(hDc) ? wglCreateContext(hDc) : nullptr;
	if (ghRc == nullptr)
		return FALSE;

	wglMakeCurrent(hDc, ghRc);

	if (glewInit() != GLEW_OK)
		return FALSE;

	RECT rc;
	GetClientRect(hWnd, &rc);
	glViewport(0, 0, rc.right - rc.left, rc.bottom - rc.top);

	gProgram = CreateShaderProgram(hWnd);
	
	glGenBuffers(1, &gVertexBuffer);
	if (gVertexBuffer != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);
		auto err = glGetError();
		if (err != GL_NO_ERROR)
			MessageBox(hWnd, _T("Error"), _T("opengl"), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	wglMakeCurrent(nullptr, nullptr);
	ReleaseDC(hWnd, hDc);

	return TRUE;
}

void CleanupOpengl()
{
	if (gVertexBuffer != 0)
	{
		glDeleteBuffers(1, &gVertexBuffer);
		gVertexBuffer = 0;
	}
	if (gProgram != 0)
	{
		glDeleteProgram(gProgram);
		gProgram = 0;
	}
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(ghRc);
	ghRc = nullptr;
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
	wcex.lpszClassName = _T("DllWindow2");
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		auto hDc = GetDC(hWnd);
		DrawGL(hDc);
		ReleaseDC(hWnd, hDc);
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

IntPtr ClassLibrary2::Class2::CreateHostedWindow(IntPtr parentHwnd, int width, int height)
{
	MyRegisterClass2();
	auto hWnd = CreateWindow(_T("DllWindow2"), _T("hosted window2"), WS_CHILD | WS_VISIBLE, 0, 0, width, height, (HWND)parentHwnd.ToPointer(), nullptr, nullptr, 0);
	InitOpengl(hWnd);
	return IntPtr(hWnd);
}

void ClassLibrary2::Class2::DestroyHostedWindow(IntPtr hWnd)
{
	CleanupOpengl();
	DestroyWindow((HWND)hWnd.ToPointer());
}

void ClassLibrary2::Class2::Effect()
{
	throw gcnew System::NotImplementedException();
}
