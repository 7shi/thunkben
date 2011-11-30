#include "stdafx.h"
#include "resource.h"

void Window::OnMouseDown(int button, WPARAM wParam, LPARAM lParam) {
    int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
    for each (auto f in MouseDown) f(button, x, y, wParam);
}

void Window::OnMouseUp(int button, WPARAM wParam, LPARAM lParam) {
    int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
    for each (auto f in MouseUp) f(button, x, y, wParam);
}

//
//  関数: Window::MyRegisterClass()
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
ATOM Window::MyRegisterClass(HINSTANCE hInstance, const tstring &windowClass)
{
    hInst = hInstance;
    this->windowClass = windowClass;

    wndProc.init(this, &Window::WndProc);

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)wndProc.getCode();
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32THUNK));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_WIN32THUNK);
    wcex.lpszClassName  = windowClass.c_str();
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   関数: Window::InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL Window::InitInstance(const tstring &title, int nCmdShow)
{
    hWnd = CreateWindow(windowClass.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  関数: Window::WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
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
    case WM_LBUTTONDOWN: OnMouseDown(1, wParam, lParam); break;
    case WM_RBUTTONDOWN: OnMouseDown(2, wParam, lParam); break;
    case WM_MBUTTONDOWN: OnMouseDown(3, wParam, lParam); break;
    case WM_XBUTTONDOWN: OnMouseDown(3 + HIWORD(wParam), LOWORD(wParam), lParam); break;
    case WM_LBUTTONUP: OnMouseUp(1, wParam, lParam); break;
    case WM_RBUTTONUP: OnMouseUp(2, wParam, lParam); break;
    case WM_MBUTTONUP: OnMouseUp(3, wParam, lParam); break;
    case WM_XBUTTONUP: OnMouseUp(3 + HIWORD(wParam), LOWORD(wParam), lParam); break;
    case WM_MOUSEMOVE:
        for each (auto f in MouseMove)
            f(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
