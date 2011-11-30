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
//  �֐�: Window::MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
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
//   �֐�: Window::InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
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
//  �֐�: Window::WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND  - �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT    - ���C�� �E�B���h�E�̕`��
//  WM_DESTROY  - ���~���b�Z�[�W��\�����Ė߂�
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
        // TODO: �`��R�[�h�������ɒǉ����Ă�������...
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
