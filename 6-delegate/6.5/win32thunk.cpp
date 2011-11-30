// win32thunk.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "win32thunk.h"

static tstring LoadTString(HINSTANCE hInstance, UINT uID) {
    const int buflen = 256;
    TCHAR buf[buflen];
    LoadString(hInstance, uID, buf, buflen);
    return buf;
}

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
INT_PTR CALLBACK        About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �����ɃR�[�h��}�����Ă��������B
    MSG msg;
    HACCEL hAccelTable;

    Window win;
    win.MyRegisterClass(hInstance, LoadTString(hInstance, IDC_WIN32THUNK));
    win.Command.push_back([&](int id, int e)->bool {
        // �I�����ꂽ���j���[�̉��:
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
    win.Paint.push_back([&](HDC hdc) {
        auto oldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
        auto oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        Rectangle(hdc, 10, 10, 40, 40);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
    });

    // �A�v���P�[�V�����̏����������s���܂�:
    if (!win.InitInstance(LoadTString(hInstance, IDS_APP_TITLE), nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32THUNK));

    // ���C�� ���b�Z�[�W ���[�v:
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

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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
