// win32thunk.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "win32thunk.h"

static tstring LoadTString(HINSTANCE hInstance, UINT uID) {
    const int buflen = 256;
    TCHAR buf[buflen];
    LoadString(hInstance, uID, buf, buflen);
    return buf;
}

class DragHandler {
    Coroutine<bool> cr;
    std::function<void()> handler;

public:
    int x, y;

    DragHandler(Window *win) {
        win->MouseMove.push_back([&](int x, int y, WPARAM) {
            if (cr.value) { this->x = x; this->y = y; cr(); }
        });
        win->MouseUp.push_back([&](int button, int x, int y, WPARAM) {
            if (cr.value) { cr.value = false; cr(); }
        });
    }

    void operator=(const decltype(handler) &h) {
        cr = handler = h;
        cr.value = false;
    }

    void start(int x, int y) {
        this->x = x;
        this->y = y;
        if (!cr.value) { cr.reset(); cr(); }
    }
};

struct Rect {
  int x, y, w, h;
  inline int r() { return x + w; }
  inline int b() { return y + h; }
  Rect(int x, int y, int w, int h): x(x), y(y), w(w), h(h){}
  bool contains(int px, int py) {
    return x <= px && px < r() && y <= py && py < b();
  }
};

// このコード モジュールに含まれる関数の宣言を転送します:
INT_PTR CALLBACK        About(HWND, UINT, WPARAM, LPARAM);

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

    Window win;
    win.MyRegisterClass(hInstance, LoadTString(hInstance, IDC_WIN32THUNK));
    win.Command.push_back([&](int id, int e)->bool {
        // 選択されたメニューの解析:
        switch (id)
        {
        case IDM_ABOUT:
            DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), win.hWnd, About);
            return true;
        case IDM_EXIT:
            DestroyWindow(win.hWnd);
            return true;
        }
        return false;
    });

    POINT pb = { 10, 10 };
    win.Paint.push_back([&](HDC hdc) {
        auto oldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
        auto oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        Rectangle(hdc, pb.x, pb.y, pb.x + 40, pb.y + 40);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
    });

    DragHandler dh(&win);
    win.MouseDown.push_back([&](int btn, int x, int y, WPARAM) {
        if (pb.x <= x && x <= pb.x + 40 && pb.y <= y && y <= pb.y + 40)
            dh.start(x, y);
    });
    dh = [&] {
        int x = dh.x, y = dh.y, px = pb.x, py = pb.y;
        while (yield(true)) {
            int oldx = pb.x, oldy = pb.y;
            pb.x = px + (dh.x - x);
            pb.y = py + (dh.y - y);
            RECT r = { min(oldx, pb.x), min(oldy, pb.y),
                max(oldx, pb.x) + 40, max(oldy, pb.y) + 40 };
            InvalidateRect(win.hWnd, &r, true);
        }
    };

    // アプリケーションの初期化を実行します:
    if (!win.InitInstance(LoadTString(hInstance, IDS_APP_TITLE), nCmdShow))
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
