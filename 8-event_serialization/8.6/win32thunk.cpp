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
    Rect(int x, int y, int w, int h): x(x), y(y), w(w), h(h) {}
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

    std::vector<Rect> rects;
    rects.push_back(Rect(10, 10, 40, 40));
    rects.push_back(Rect(60, 60, 40, 40));

    win.Paint.push_back([&](HDC hdc) {
        auto oldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
        auto oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        for (auto it = rects.begin(); it != rects.end(); it++)
            Rectangle(hdc, it->x, it->y, it->r(), it->b());
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
    });

    DragHandler dh(&win), dh2(&win);
    decltype(rects.rbegin()) sel;
    win.MouseDown.push_back([&](int btn, int x, int y, WPARAM) {
        for (auto it = rects.rbegin(); it != rects.rend(); it++) {
            if (it->contains(x, y)) {
                sel = it;
                if (it->r() - x <= 5 && it->b() - y <= 5)
                    dh2.start(x, y);
                else
                    dh.start(x, y);
                return;
            }
        }
        rects.push_back(Rect(x, y, 2, 2));
        sel = rects.rbegin();
        dh2.start(x, y);
    });
    dh = [&] {
        int x = dh.x, y = dh.y, rx = sel->x, ry = sel->y;
        while (yield(true)) {
            auto old = *sel;
            sel->x = rx + (dh.x - x);
            sel->y = ry + (dh.y - y);
            RECT r = { min(old.x, sel->x), min(old.y, sel->y),
                max(old.r(), sel->r()), max(old.b(), sel->b()) };
            InvalidateRect(win.hWnd, &r, true);
        }
    };
    dh2 = [&] {
        int x = dh2.x, y = dh2.y, rw = sel->w, rh = sel->h;
        while (yield(true)) {
            auto old = *sel;
            sel->w = max(rw + (dh2.x - x), 2);
            sel->h = max(rh + (dh2.y - y), 2);
            RECT r = { sel->x, sel->y,
                max(old.r(), sel->r()), max(old.b(), sel->b()) };
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
