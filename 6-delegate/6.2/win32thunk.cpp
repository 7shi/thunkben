// win32thunk.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "win32thunk.h"
#include "xbyak/xbyak.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

template <class T, class TA1, class TA2, class TA3, class TA4, class TR>
struct StdCallThunk : public Xbyak::CodeGenerator {
    void init(T *t, TR(T::*p)(TA1, TA2, TA3, TA4)) {
        mov(ecx, reinterpret_cast<intptr_t>(t));
        jmp(*reinterpret_cast<void **>(&p));
    }
};

class Window {
protected:
    StdCallThunk<Window, HWND, UINT, WPARAM, LPARAM, LRESULT> wndProc;
public:
    HWND hWnd;
    std::list<std::function<bool(int, int)>> Command;
    std::list<std::function<void(HDC)>> Paint;
    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32THUNK, szWindowClass, MAX_LOADSTRING);
	Window win;
	win.MyRegisterClass(hInstance);
    win.Command.push_back([&](int id, int e)->bool {
        // 選択されたメニューの解析:
        switch (id)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), win.hWnd, About);
            return true;
        case IDM_EXIT:
            DestroyWindow(win.hWnd);
            return true;
        }
        return false;
    });
    win.Paint.push_back([&](HDC hdc) {
        auto oldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
        auto oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        Rectangle(hdc, 10, 10, 40, 40);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
    });

	// アプリケーションの初期化を実行します:
	if (!win.InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32THUNK));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM Window::MyRegisterClass(HINSTANCE hInstance)
{
	wndProc.init(this, &Window::WndProc);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)wndProc.getCode();
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32THUNK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32THUNK);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
BOOL Window::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
        for each (auto f in Command)
            if (f(LOWORD(wParam), HIWORD(wParam)))
                return 0;
        return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: 描画コードをここに追加してください...
        for each (auto f in Paint) f(hdc);
        EndPaint(hWnd, &ps);
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
